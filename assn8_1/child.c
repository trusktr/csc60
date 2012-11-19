// child.c - Joe Pea - Assignment 8 - CSC 60 - Weide Chang
// CR
// CSc 60 IPC: Semaphores

#include "common.h"
#include <stdbool.h>

int delta(bool min, bool max) {
	int randNumber;
	int direction;
	if (min && max) {
		printf("Error: Delta(): min and max cannot both be true.");
		exit(66);
	}

	if (min) {
		direction = 1;
	}
	else if (max) {
		direction = -1;
	}
	else { // min and max are false
		randNumber = rand() % 2;
		if (randNumber == 0)
			direction = -1;
		else
			direction = 1;
	}

	return direction;
}

//***************************************************************************
// code below is used for multiple child processes
//***************************************************************************
void main(int argc, char *argv[])
{
   int row, col, pipe_end, symbol, usec;
   sem_t *sem_video, *sem_me;
   char str[100];     // str for common usage

sprintf(str, "%s%s%s%u%s", "echo '-- debug ", symbol=='C'?"chaser":"runner", " -- PID: ", getpid(), "'  > /dev/pts/5");
system(str);

   row = atoi( argv[1] );
   col = atoi( argv[2] );
   pipe_end = atoi( argv[3] );
   symbol = argv[4][0];    // my assigned symbol, 'C' or 'R'

sprintf(str, "%s%s%s%u%s%u%s", "echo '-- debug ", symbol=='C'?"chaser":"runner", " -- Starting Row,Col: ", row, ",", col, "'  > /dev/pts/5");
system(str);

   sprintf(str, "%u", getppid()); // get parent pid to build key str
   sem_video = sem_open( str, O_CREAT, S_IRWXU, 0 ); // obtain semaphore

   sprintf(str, "%u", symbol+getppid()); // get parent pid PLUS MY SYMBOL to build key str
   sem_me = sem_open( str, O_CREAT, S_IRWXU, 0 ); // obtain semaphore

	/*
	 * Let's initiate some randomness based on the current time,
	 * then seed random randomly to be more random.
	 */
	srand( (unsigned int)time(NULL) );
/*	int k; for (k=0; k<rand()%100+50; k++) srand( rand() );*/

char tmp[100]; strcpy(tmp, str);
sprintf(str, "%s%s%s%s%s", "echo '-- debug ", symbol=='C'?"chaser":"runner", " -- sem key: ", tmp, "'  > /dev/pts/5");
system( str );

   while(1)
   {

system( symbol=='C'?"echo -- debug chaser 1 > /dev/pts/5":"echo -- debug runner 1 > /dev/pts/5" );
      sem_wait( sem_me ); // wait until parent post (so can syn children)

system( symbol=='C'?"echo -- debug chaser 2 > /dev/pts/5":"echo -- debug runner 2 > /dev/pts/5" );

      PutChar( row, col, symbol, sem_video );     // show symbol 1st

system( symbol=='C'?"echo -- debug chaser 3 > /dev/pts/5":"echo -- debug runner 3 > /dev/pts/5" );


sprintf(str, "%s%s%s%i%s", "echo '-- debug ", symbol=='C'?"chaser":"runner", " -- row: ", row, "'  > /dev/pts/5");
system( str );
sprintf(str, "%s%s%s%i%s", "echo '-- debug ", symbol=='C'?"chaser":"runner", " -- col: ", col, "'  > /dev/pts/5");
system( str );
      //write to pipe... tell parent where I'm (row)
      write(pipe_end, &row, sizeof(int));
      //write to pipe... tell parent where I'm (col)
      write(pipe_end, &col, sizeof(int));

system( symbol=='C'?"echo -- debug chaser 4 > /dev/pts/5":"echo -- debug runner 4 > /dev/pts/5" );


      usec = USEC;      // sleep period may be 1/2/or 3 portions of USEC
      usleep(usec);

system( symbol=='C'?"echo -- debug chaser 5 > /dev/pts/5":"echo -- debug runner 5 > /dev/pts/5" );

      PutChar( row, col, ' ', sem_video );  // put out a space (erase)

system( symbol=='C'?"echo -- debug chaser 6 > /dev/pts/5":"echo -- debug runner 6 > /dev/pts/5" );

      if(row == 1) row += delta(true, false);
      else if(row == MAX_ROW) row += delta(false, true);
      else row += delta(false, false);
      if(col == 1) col += delta(true, false);
      else if(col == MAX_COL) col += delta(false, true);
      else col += delta(false, false);

system( symbol=='C'?"echo -- debug chaser 7 > /dev/pts/5":"echo -- debug runner 7 > /dev/pts/5" );
   }
}

