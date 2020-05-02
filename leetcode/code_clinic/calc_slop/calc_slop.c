/*
 * demo C code 
 * Original from Dan Gookin, here update for easy demo 
 * Return the coefficient of the slope of a set of collected weather data
 * Optional: Graph the data
 *
 * NOTE: This code assumes that the data files provided exist un-modified
 * in the same directory as the code's program. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* the width of the chart output is adjusted here */
#define CHART_WIDTH 64

/* This structure helps convert the date stamp string
   into a time_t value */
struct datetime {
	int year;
	int month;
	int day;
	int hour;
	int min;
	int sec;
};

/* function prototypes must appear after structure definition */
void chart(time_t *e,float *b,int c);
double linear_regression(time_t *independent,float *dependent,int size);
int read_data(struct datetime start,struct datetime finish,float **b,time_t **e);
time_t get_epoch(char *d, char *t);
void check_input(struct datetime s, struct datetime e);

/*
 * Main function
 * Initialize the storage (fake arrays), collect date range, process data, spit out results
 */
#define SIZE_ARRAY  100
 
int main()
{
	struct datetime start,finish;
	int records;
	double coef;
	float *bp;
	time_t *epoch;

	/* initialize "array" to SIZE_ARRAY elements */
	bp = (float *)malloc(SIZE_ARRAY*sizeof(float));
	epoch = (time_t *)malloc(SIZE_ARRAY*sizeof(time_t));
	if(bp==NULL || epoch==NULL)
	{
		fprintf(stderr,"Unable to allocate storage\n");
		exit(1);
	}

	/* Read start and finish date ranges */
	/* I loathe scanf(), but it's excellent for pulling out integer chunks and handles
	   bad input well. */
	printf("Date/time ranges are from 2012 01 01 : 00 00 through 2015 06 04 : 01 09\n");
	printf("Starting date and time (YYYY MM DD HH mm): ");
	//scanf("%4d %2d %2d %2d %2d", &start.year, &start.month, &start.day, &start.hour, &start.min);
	//update direct from string 
	sscanf("2012 01 01 : 00 00","%4d %2d %2d %2d %2d", &start.year, &start.month, &start.day, &start.hour, &start.min);
	start.sec = 0;
	printf("Ending date and time (YYYY MM DD HH mm): ");
	//scanf("%4d %2d %2d %2d %2d", &finish.year, &finish.month, &finish.day, &finish.hour, &finish.min);
	sscanf("2014 06 04 : 00 00","%4d %2d %2d %2d %2d", &finish.year, &finish.month, &finish.day, &finish.hour, &finish.min);
	finish.sec = 0;

	/* Validate input */
	check_input(start,finish);

	/* Open the file(s) to pull out barometric pressure data */
	records = read_data(start,finish,&bp,&epoch);
	printf("Read %d records\n",records);
	if(records==0)
		return(0);

	/* Obtain the coefficient of the slope */
	coef = linear_regression(epoch,bp,records);
	printf("The coefficient of the slope for barometric pressure is %g\n",coef);

	/* Graph the results */
	chart(epoch,bp,records);

	return(0);
}

/*
 * Draw a (crude) chart of the results
 */
void chart(time_t *e,float *b,int c)
{
	int x,rows;
	float y,interval,pressure,step;
	struct tm *t;
	time_t s,f;
		/* 32.0 is the highest recorded barometric pressure on record */
	const float max_pressure = 32.0;
		/* 26.0 is near the lowest recorded bp on record */
	const float min_pressure = 26.0;

	step = (max_pressure-min_pressure)/20;

	/* Draw the top part (graph) of the chart */
	if(c<CHART_WIDTH)	/* handle a narrow chart */
	{
		/* print chart */
		for(rows=0;rows<19;rows++)
		{
			pressure = max_pressure-(rows*step);
			printf(" %.2f |",pressure);
			/* if the current row contains data chunks greater than the value of the
			   y access, print an *, space otherwise */
			for(y=0.0;y<(float)c;y+=1.0)
			{
				if( *(b+(int)y) > pressure )
					putchar('*');
				else
					putchar('.');
			}
			putchar('\n');
		}
	}
	else	/* This condition is the more popular of the two */
	{
		interval = (float)c/CHART_WIDTH;

		/* print chart */
		for(rows=0;rows<19;rows++)
		{
			pressure = max_pressure-(rows*step);
			printf(" %.2f |",pressure);
			/* Variable y is a float type as it more accurately compresses lots of data
			   into the CHART_WIDTH. */
			for(y=0.0;y<(float)c;y+=interval)
			{
				if( *(b+(int)y) > pressure )
					putchar('*');
				else
					putchar('.');
			}
			putchar('\n');
		}
	}

	/* print chart bottom */
	printf("       +");
	/* ensure width is adjustable */
	for(x=0;x<CHART_WIDTH;x++)
		putchar('-');
	putchar('\n');

	/* fill time structures with read data timestamps */
	/* It was easier on my brain to separate the starting and final timestamps
	   into time_t variables s and f (respectively) as opposed to using some weirdo
	   pointer notation in the localtime() function.
	   Also, TIL that you can only call localtime() once, which is why the printf()
	   statement is split in two, one for each timestamp. */
	s = *(e+0);
	f = *(e+c-1);
	t = localtime(&s);
	printf("        %d %02d %02d %02d:%02d:%02d%*c",
			t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec,
			CHART_WIDTH-38,' '
		  );
	t = localtime(&f);
	printf("%d %02d %02d %02d:%02d:%02d\n",
			t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec
		  );
}

/*
 * Determine the linear regression of the data
 * Inspired by Chris Webb's http://www.code-in-c.com/linear-regression-fitting-a-line-to-data/
 * I have no idea how this works, other than following along with several tutorials.
 */
double linear_regression(time_t *independent,float *dependent,int size)
{
    double independent_mean,dependent_mean,products_mean,mean_of_squares,variance;
    double squares[size];
	double total;
	int x;

	/* obtain the independent mean */
	total = 0.0;
	for(x=0;x<size;x++)
		total+=*(independent+x);
    independent_mean = total/size;

	/* obtain the dependent mean */
	total = 0.0;
	for(x=0;x<size;x++)
		total+=*(dependent+x);
    dependent_mean = total/size;

	/* obtain the mean of products */
	total=0.0;
    for(x=0;x<size;x++)
		total+=( *(independent+x) * *(dependent+x));
	products_mean = total/size;

	/* obtain the variance */
    for(x=0;x<size;x++)
        squares[x] = ( *(independent+x) * *(independent+x) );
	/* process the mean of the squares */
	total = 0.0;
	for(x=0;x<size;x++)
		total+=squares[x];
    mean_of_squares = total/size;
    variance = mean_of_squares - independent_mean*independent_mean;

    return((products_mean-(independent_mean*dependent_mean))/variance);

	/* the "y intercept" value is calculated in the original function,
	   but not necessary here. Refer to Chris' website for details */
}

/*
 * Read Data function
 * Cull through the data files for matching dates and pull out bp and
 * time data
 * This code assumes the files are original and unmodified
 * You must pass a pointer-pointer ** (address-of-pointer) so that the
 * realloc() function properly enlarges the buffer. If you pass only
 * the pointer, the memory location can get lost.
 */
int read_data(struct datetime start,struct datetime finish,float **b,time_t **e)
{
	/* These are the unmodified filenames as unpacked from the archive
	   I suppose I could have concatenated them all, but chose not to */
	const char *filenames[4] = {
		"data/Environmental_Data_Deep_Moor_2012.dat",
		"data/Environmental_Data_Deep_Moor_2013.dat",
		"data/Environmental_Data_Deep_Moor_2014.dat",
		"data/Environmental_Data_Deep_Moor_2015.dat"
	};
	/* This structure matches the way the CSV file splits the timestamp
	   into separate text fields */
	struct timestamp {
		char date[11];
		char time[6];
	} begin, end;
	/* This structure matches the 9 fields in each row of the CSV file */
	struct weather {
		char date[11];
		char time[6];
		float air_temp;
		float bar_press;
		float dew_point;
		float humidity;
		float wind_dir;
		float wind_gust;
		float wind_speed;
	} row;
	/* The first line of each file is 104 characters long, used to store
	   header information. It's read in this function and discarded */
	char buffer[105];
	int f,toggle,count;
	FILE *data;

	/* translate and store date and time strings */
	/* The starting and finishing timestamp is transferred to the local
	   timestamp structure. The timestamp structure makes it easier to compare
	   date/time strings from the CSV files */
	sprintf(begin.date,"%4d_%02d_%02d", start.year, start.month, start.day);
	sprintf(begin.time,"%02d:%02d", start.hour, start.min);
	sprintf(end.date,"%4d_%02d_%02d", finish.year, finish.month, finish.day);
	sprintf(end.time,"%02d:%02d", finish.hour, finish.min);

	/* initialize the count, used to track "array" elements */
	count = 0;
	/* set the toggle for reading data */
	toggle = 0;

	/* cycle through the requested years to match files */
	/* This loop processes each file based in the start and
	   finish year provided. */
	for(f=start.year-2012;f<=finish.year-2012;f++)
	{
		/* open the data file */
		data = fopen(filenames[f],"r");
		if(data==NULL)
		{
			fprintf(stderr,"Unable to open '%s'\n",filenames[f]);
			exit(1);
		}
		/* discard the first line / header row */
			/* again, this code assumes that the file content/format is unchanged */
		fgets(buffer,105,data);
		/* read remaining lines */
		/* After the first line, each row contains 9 data elements. These are continually
		   read. The initial pass looks for a matching start string. */
		while(!feof(data))
		{
			/* 9 items per line */
			fscanf(data,"%s %s %f %f %f %f %f %f %f",
					row.date,
					row.time,
					&row.air_temp,
					&row.bar_press,
					&row.dew_point,
					&row.humidity,
					&row.wind_dir,
					&row.wind_gust,
					&row.wind_speed
				  );
			/* if the toggle is active, ignore the date check */
			/* The toggle is set only when the starting date and time has matched */
			if(toggle==0)
			{
				/* check for the start date and time */
				/* if strncmp() is <0, then begin.time has passed */
				if(strcmp(begin.date,row.date)==0 && strncmp(begin.time,row.time,5)<=0)
				{
					/* set the toggle */
					toggle = 1;
				}
			}

			/* when toggle is active */
			else
			{
				/* add barometric pressure value to the "array" */
				/* It was on a later revision of this code that I added the timestamp
				   value to a second pointer. Originally, both were stored in a single
				   structure, but the structure notation got too hairy to follow, so
				   I split them into two "arrays." */
				*(*b+count) = row.bar_press;
				*(*e+count) = get_epoch(row.date,row.time);
				count++;

				/* do we need more elements in the "array"? */
				if(count%100==0)
				{
					/* Here is where the ** pointer notation is important. Because the
					   pointer variable dwells in the main() function, access here must
					   be made indirectly. */
					*b = realloc(*b,(count+100)*sizeof(float));
					*e = realloc(*e,(count+100)*sizeof(time_t));
					if(b==NULL || e==NULL)
					{
						fprintf(stderr,"Unable to reallocate memory.\n");
						exit(1);
					}
				}

				/* test for end condition */
				/* When the end date matches and the end time as passed, stop */
				if(strcmp(end.date,row.date)==0 && strncmp(end.time,row.time,5)<=0)
					   break;
				/* Because the timestamp data can be inconsistent or missing, this second
				   text checks to see whether the end date has passed. If so, the loop
				   stops. Otherwise, the code would loop until the end of the file. */
				if(strcmp(end.date,row.date)<0)
					break;
			}
		}
		fclose(data);
	}
	return(count);
}

/*
 * Translate the CSV file's date and time stamp strings
 * into epoch time value. This is my attempt to provide some
 * dependent data for the linear regression calculation.
 */
time_t get_epoch(char *d, char *t)
{
	char timestamp[20];
	char *t_offset;
	struct datetime ts_temp;
	time_t now;
	struct tm *ts;

	/* build single timestamp string */
	strcpy(timestamp,d);
	strcat(timestamp," ");
	strcat(timestamp,t);

	/* split string */
	/* Again, this assumes a consistent format: YYYY_MM_DD hh:mm:ss */
	/* Given the preset positions of the timestamp, these statements
	   set null characters after each significant value in the string */
	timestamp[4] = '\0';
	timestamp[7] = '\0';
	timestamp[10] = '\0';
	timestamp[13] = '\0';
	timestamp[16] = '\0';

	/* extract int values from timestamp strings */
	/* As before, these statements assume the string is properly formatted
	   and chopped. The t_offset pointer slices through the string
	   to each value's predetermined position */
	t_offset = timestamp;
	ts_temp.year = atoi(t_offset);
	ts_temp.month = atoi(t_offset+5);
	ts_temp.day = atoi(t_offset+8);
	ts_temp.hour = atoi(t_offset+11);
	ts_temp.min = atoi(t_offset+14);
	ts_temp.sec = atoi(t_offset+17);

	/* fill tm structure with current values */
	/* To create an epoch time_t value, you first fill a tm structure,
	   but that structure must contain valid data. To do so, I first
	   call time() and set the current time into the structure to ensure
	   that all available fields are filled */
	time(&now);
	ts = localtime(&now);
	/* update tm structure with recorded values */
	/* Only those relevant members are filled with the new data. Other values
	   not filled aren't relevant to this program's goal; they're ignored */
	ts->tm_year = ts_temp.year-1900;
	ts->tm_mon = ts_temp.month-1;
	ts->tm_mday = ts_temp.day;
	ts->tm_hour = ts_temp.hour;
	ts->tm_min = ts_temp.min;
	ts->tm_sec = ts_temp.sec;

	/* create event epoch time value and return*/
	return(mktime(ts));
}

/*
 * Check Input
 * Do preliminary bounds-checking for input
 * This is a lot of code, but necessary to avoid reading errors later
 */
void check_input(struct datetime s, struct datetime e)
{
	/* ensure starting year value is within range */
	if(s.year<2012 || s.year>2015)
	{
		fprintf(stderr,"Start year '%d' is out of range.\n",s.year);
		exit(1);
	}

	/* Ensure finishing year value is within range given starting year */
	if(e.year<s.year || e.year>2015)
	{
		fprintf(stderr,"End year %d must be after %d.\n",e.year,s.year);
		exit(1);
	}

	/* Ensure month values are within range */
	if(s.month<1 || e.month<1 || s.month>12 || e.month>12)
	{
		fprintf(stderr,"Month value is out of range\n");
		exit(1);
	}

	/* Don't worry about months with 30 or fewer days at this point */
	/* Yes, this is sloppy and I could code it specifically so that each
	   month is properly represented. But have you seen the data! It's wildly
	   inconsistent, so I can afford a wee bit of slop */
	if(s.day<1 || e.day<1 || s.day>31 || e.day>31)
	{
		fprintf(stderr,"Day value is out of range\n");
		exit(1);
	}

	/* Check hour values */
	if(s.hour<0 || e.hour<0 || s.hour>23 || e.hour>23)
	{
		fprintf(stderr,"Hour value is out of range\n");
		exit(1);
	}

	/* Check minute values */
	if(s.min<0 || e.min<0 || s.min>59 || e.min>59)
	{
		fprintf(stderr,"Minute value is out of range\n");
		exit(1);
	}

	/* Confirm that values within a single year are ordered properly */
	if(s.year == e.year)
	{
		/* Are the months in sequence? */
		if(s.month > e.month)
		{
			fprintf(stderr,"Month values out of sequence\n");
			exit(1);
		}
		if(s.month == e.month)
		{
			/* Are the days in sequence? */
			if(s.day > e.day)
			{
				fprintf(stderr,"Day values out of sequence\n");
				exit(1);
			}
			/* Are the hours in sequence? */
			if(s.day == e.day)
			{
				if(s.hour > e.hour)
				{
					fprintf(stderr,"Hours out of sequence\n");
					exit(1);
				}
				/* Are the minutes in sequence? */
				if(s.hour == e.hour)
					if(s.min > e.min)
					{
						fprintf(stderr,"Minutes out of sequence\n");
						exit(1);
					}
				/* I don't bother with seconds */
			}	/* end hour check */
		}	/* end month check */
	}	/* end year check */
}

