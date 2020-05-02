/*
 * Dashboard ncurses
 * Dan Gookin
 * Create a dashboard to display the status of a process at the current
 * simulated time.
 * Optional: Allow interactive control of values
 *
 * NOTE: This code assumes that the data files provided exist un-modified
 * in the same directory as the code's program.
 */

#include <ncurses.h>
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
	/* ncurses constants for positioning and such */
#define TITLE "| C Dashboard by Dan Gookin |"
#define MAIN_WIDTH 80
#define MAIN_HEIGHT 24
#define ENGINE_OFFSET 32
#define COLORS_OFFSET 65


/* function prototypes */
void extract(char *s,struct engine *d);
time_t epoch(char *ts);

int main()
{
	FILE *df;
	char buffer[LINEBUFFER];
	char timestring[20];
	struct engine data;
	time_t currently,temp;
	WINDOW *main,*datebox,*redbox,*greenbox,*bluebox;
	int y,percentage;

	/* start with Ncurses initialization */
	initscr();
	/* set colors */
	start_color();
	init_pair(0,COLOR_WHITE,COLOR_BLACK);
	init_pair(1,COLOR_WHITE,COLOR_RED);
	init_pair(2,COLOR_WHITE,COLOR_GREEN);
	init_pair(3,COLOR_WHITE,COLOR_BLUE);
	init_pair(4,COLOR_BLACK,COLOR_CYAN);

	refresh();

	/* confirm that the terminal window is large enough for the output */
	if(LINES<MAIN_HEIGHT || COLS<MAIN_WIDTH)
	{
		addstr("This program is optimized for a screen\n");
		printw("at least %d rows by %d columns.\n",
				MAIN_HEIGHT,
				MAIN_WIDTH
			  );
		addstr("Press Enter to quit.");
		getch();
		endwin();
		exit(1);
	}

	/* open the data file */	
	df = fopen(DATAFILE,"r");
	if(df==NULL)
	{
		printw("Unable to open '%s'\n",DATAFILE);
		addstr("Press Enter to quit.");
		getch();
		endwin();
		exit(1);
	}

	/* display keyboard blocking */
	nodelay(stdscr,TRUE);
	noecho();

	/* read and discard the first line, the CSV header row */
	fgets(buffer,LINEBUFFER,df);

	/* create the window */
	main = newwin(MAIN_HEIGHT,MAIN_WIDTH,0,0);
	if(main==NULL)
	{
		addstr("Unable to allocate main window.");
		getch();
		endwin();
		return(1);
	}

	/* draw a pretty border around the screen with a title */
	box(main,0,0);
		/* center the title */
	mvwaddstr(
			main,
			0,
			(MAIN_WIDTH-strlen(TITLE))/2,
			TITLE
			);
	/* draw the date box */
	datebox = subwin(main,20,22,2,5);
	box(datebox,0,0);
		/* this gives them something to look at while the file loads */
	wrefresh(main);

	/*************
	 * MAIN LOOP *
	 *************/
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
			/* fill in the date/time stamp box */
			for(y=1;y<19;y++)
			{
				temp = data.timestamp - (y - 9L);
				strftime(timestring,20,"%F %T",localtime(&temp));
				/* highlight row 9 */
				if(y==9)
				{
					wattron(datebox,A_REVERSE);
					mvwprintw(datebox,y,1,"%s",timestring);
					wattroff(datebox,A_REVERSE);
				}
				else
				{
					mvwprintw(datebox,y,1,"%s",timestring);
				}
			}
		
			/* setup the pulsometer */
			mvwprintw(main,2,43,"Pulsometer: %3d",data.pulsometer);

			/* engine efficiency graph */
			percentage = 100;
			for(y=5;y<21;y+=3,percentage-=10)
				mvwprintw(main,y,ENGINE_OFFSET,"%3d%%",percentage);
			mvwprintw(main,22,ENGINE_OFFSET-3,"Engine Efficiency %.2f%%",data.efficiency);
			/* graph */
			for(y=5;y<21;y++)
			{
				if((int)data.efficiency > 100-((y-4)*3))
				{
					wattrset(main,COLOR_PAIR(4));
					mvwaddstr(main,y,ENGINE_OFFSET+7,"       ");
				}
				else
				{
					wattrset(main,COLOR_PAIR(0));
					mvwaddstr(main,y,ENGINE_OFFSET+7,"       ");
				}
			}
			wattrset(main,COLOR_PAIR(0));

			/* draw the color boxes */
				/* red */
			redbox = subwin(main,3,5,6,COLORS_OFFSET);
			wbkgd(redbox,COLOR_PAIR(1)|' ');
			mvwaddstr(main,7,COLORS_OFFSET-5,"Red:");
			attron(A_BOLD);
			mvwprintw(redbox,1,1,"%3d",data.red);
				/* green */
			greenbox = subwin(main,3,5,11,COLORS_OFFSET);
			wbkgd(greenbox,COLOR_PAIR(2)|' ');
			mvwaddstr(main,12,COLORS_OFFSET-7,"Green:");
			attron(A_BOLD);
			mvwprintw(greenbox,1,1,"%3d",data.green);
				/* blue */
			bluebox = subwin(main,3,5,16,COLORS_OFFSET);
			wbkgd(bluebox,COLOR_PAIR(3)|' ');
			mvwaddstr(main,17,COLORS_OFFSET-6,"Blue:");
			attron(A_BOLD);
			mvwprintw(bluebox,1,1,"%3d",data.blue);
	
			/* update and show */
			wrefresh(main);
			wrefresh(datebox);
			wrefresh(redbox);
			wrefresh(greenbox);
			wrefresh(bluebox);

			/* wait for the next tick */
			while(data.timestamp==currently)
			{
				/* check for buffered key press */
				if(getch() != ERR)
					break;
				/* keep fetching the current time */
				time(&currently);
			}
		}
	}

	fclose(df);

	/* shutdown ncurses */
	endwin();
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

