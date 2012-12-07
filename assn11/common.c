// common.c
// AlphaDerby - Shared Memory (TwoTermsPick)

#include "common.h"

/////////////////////////// below common routine ///////////////////////////
void PutChar(int row, int col, char ch, sem_t *sem)
{
   char str[30];

   sprintf(str, "%c[%d;%dH", 27, row, col); // build str: move cursor to row-col

   sem_wait( sem );            // seize video access
   write(1, str, strlen(str)); // move cursor
   write(1, &ch, 1);           // write out the character
   sem_post( sem );            // release video usage
}

void Flash()
{
   char str[30];

   sprintf(str, "%c[?5h", 27); // inverse video mode
   write(1, str, strlen(str));
   usleep(100000);             // delay for .1 sec
   sprintf(str, "%c[?5l", 27); // normal video mode
   write(1, str, strlen(str));

   sprintf(str, "%c[28;1H", 27 ); // move cursor to row 28, col 1
   write(1, str, strlen(str));

   sprintf(str, "%c[?25h", 27); // cursor on, off is [?25l
   write(1, str, strlen(str));
}

void InitScr(sem_t *sem)
{
   int i;
   char str[30];

   sprintf(str, "%c[2J", 27);     // clear screen
   write( 1, str, strlen(str) );

   sprintf(str, "%c[?25l", 27);   // cursor off, on is [?25h
   write( 1, str, strlen(str) );

   for( i=1; i<=26; i++) // show a dot on each of 26 rows, col MAX_COL
   {
      sprintf(str, "%c[%d;%dH", 27, i, MAX_COL); // row i, col MAX_COL
      write( 1, str, strlen(str) );              // move cursor there
      write( 1, ".", 1 );                        // show a dot
   }
}

