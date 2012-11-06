// child.c - Joe Pea - Assignment 8 - CSC 60 - Weide Chang
// CR
// CSc 60 IPC: Semaphores

#include "common.h"

//***************************************************************************
// code below is used for multiple child processes
//***************************************************************************
void main(int argc, char *argv[])
{
   int row, col, pipe_end, symbol, row_delta, col_delta, usec;
   sem_t *sem_video, *sem_me;
   char str[20];     // str for common usage

   row = atoi( argv[1] );
   col = atoi( argv[2] );
   pipe_end = atoi( argv[3] );
   symbol = argv[4][0];    // my assigned symbol, 'C' or 'R'

   sprintf(str, "%d", getppid()); // get parent pid to build key str
   sem_video = sem_open( str, O_CREAT ); // obtain semaphore

   sprintf(str, "%d", symbol+getppid()); // get parent pid PLUS MY SYMBOL to build key str
   sem_me = sem_open( str, O_CREAT ); // obtain semaphore

   srand( getpid() );      // seed rand()

   while(1)
   {
      sem_wait( sem_me ); // wait until parent post (so can syn children)

      PutChar( row, col, symbol, sem_video );     // show symbol 1st

      //write to pipe... tell parent where I'm (row)
      write(pipe_end, "%i", row);
      //write to pipe... tell parent where I'm (col)
      write(pipe_end, "%i", col);
      

      usec = 300000;      // sleep period may be 1/2/or 3 portions of USEC
      usleep(usec);

      PutChar( row, col, ' ', sem_video );  // put out a space (erase)

      row_delta = rand() % 3 -1; // row is to change by -1, 0, or 1
      col_delta = rand() % 3 -1; // col is to change by -1, 0, or 1

      row += row_delta;      // apply row change
      col += col_delta;      // apply col change

      if(row < 0) row = 0;      			// off top end, stay
      if(row > MAX_ROW) row = MAX_ROW;     // off bottom end, stay
      if(col < 0 ) col = 0;     			// off left end, stay
      if(col > MAX_COL ) col = MAX_COL;     // off right end, stay
   }
}

