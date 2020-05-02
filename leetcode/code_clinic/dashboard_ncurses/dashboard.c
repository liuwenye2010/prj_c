/*
 * C example 
 * from Dan Gookin
 * Create a dashboard to display the status of a process at the current
 * simulated time.
 * Optional: Allow interactive control of values
 *
 * NOTE: This code assumes that the data files provided exist un-modified
 * in the same directory as the code's program.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* structure to hold line data */
struct engine {
	time_t timestamp;
	int pulsometer;
	float efficiency;
	unsigned char red;
	unsigned char blue;
	unsigned char green;
};

/* constants */
	/* the CSV, plain text version of the datafile */
#define DATAFILE "dashBoardData.csv"
	/* each line should fit comfortably in a 128-byte buffer */
#define LINEBUFFER 128

/* function prototypes */
void extract(char *s,struct engine *d);
time_t epoch(char *ts);

int main()
{
	FILE *df;
	char buffer[LINEBUFFER];
	char timestring[20];
	struct engine data;
	time_t currently;

	df = fopen(DATAFILE,"r");
	if(df==NULL)
	{
		fprintf(stderr,"Unable to open '%s'\n",DATAFILE);
		exit(1);
	}

	/* read and discard the first line, the CSV header row */
	fgets(buffer,LINEBUFFER,df);

	/* initially search for the current time of day */
	time(&currently);
	/* read until the end-of-file */
	while(!feof(df))
	{
		fgets(buffer,LINEBUFFER,df);
		/* remove individual fields from the CSV file */
		extract(buffer,&data);
		/* Stop reading the file when the current time record is encountered */
		if(data.timestamp==currently)
		{
			/* concoct a date/time stamp */
			strftime(timestring,20,"%F %T",localtime(&data.timestamp));
			/* display individual "now" values */
			printf("%s Pulsometer:%3d Engine Efficiency:%f Red:%3d Blue:%3d Green:%3d\n",
					timestring,
					data.pulsometer,
					data.efficiency,
					data.red,
					data.blue,
					data.green
				  );
			/* wait for the next tick */
			while(data.timestamp==currently)
				time(&currently);
		}
	}

	fclose(df);

	return(0);
}

/*
 * read the buffer and extract the relevant data
 */
void extract(char *s,struct engine *d)
{
	char timestring[9];
	char *sptr;

	/* grab the current time portion of the string */
	sptr = s;
	strncpy(timestring,sptr+11,8);
	/* the strncpy function does not terminate */
	timestring[8] = '\0';
	/* obtain the current (today's) epoch time value */
	d->timestamp = epoch(timestring);

	/* convert commas in the buffer to null characters */
	while(*(sptr))
	{
		if(*sptr==',')
			*sptr='\0';
		sptr++;
	}

	/* using given offsets, grab the stringlets */
	/* 2018-01-01T08:00:00Z,1,79.66026608,0,125,3 */
	sptr = s+21;
	d->pulsometer = atoi(sptr);
	/* skip over to the next value */
	while(*sptr++ != '\0')
		;
	d->efficiency = strtof(sptr,NULL);
	/* skip over to the next value */
	while(*sptr++ != '\0')
		;
	d->red = atoi(sptr);
	/* skip over to the next value */
	while(*sptr++ != '\0')
		;
	d->blue = atoi(sptr);
	/* skip over to the next value */
	while(*sptr++ != '\0')
		;
	d->green = atoi(sptr);
}

/*
 * using the time from the CSV file, calculate the epoch time
 * for the current, local time equivalent
 * This function is similar to the get_epoch() function found in
 * my solution for Code Clinic Problem 1: Obtain the current
 * epoch time value, fill a tm structure, update the structure,
 * then calculate the proper epoch time.
 */
time_t epoch(char *s)
{
	time_t now;
	struct tm *ts;
	int hour,minute,second;

	/* split the string: 08:00:00 */
	*(s+2) = '\0';
	*(s+5) = '\0';
	/* extract the values */
	hour = atoi(s);
	minute = atoi(s+3);
	second = atoi(s+6);

	/* fill the tm structure with the current time */
	time(&now);
	ts = localtime(&now);
	/* update the tm structure with the hour-min-sec values
	   read from the CSV file */
	ts->tm_hour = hour;
	ts->tm_min = minute;
	ts->tm_sec = second;

	/* make the new epoch time value and return it */
	return(mktime(ts));
}

