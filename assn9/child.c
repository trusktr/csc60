// child.c
// AlphaDerby

#include "common.h"

int main(int argc, char *argv[]) // need to know the letter symbol
{
   char symbol, str[20];
   sem_t *sem;
   int i, row;

   symbol = argv[1][0];
   row = (int)symbol - 64;     // use symbol to set row to 1 for 'A', 2 'B', 3 'C', etc.

   sprintf( str, "%u", getppid() );     // build key str from parent pid
   sem = sem_open( str, O_CREAT, S_IRWXU, 1 );   // get semaphore (sem_t *) that parent has openned
   if( sem == SEM_FAILED )
   {
      perror("Error opening semaphore: ");
      return 0;
   }

   srand( getpid() * (unsigned int)time(NULL) ); // seed rand() with my pid

   for(i=1; i<=MAX_COL; i++ ) // for each of MAX_COL columns
   {
      PutChar( row, i, symbol, sem ); // show my symbol at the column (row is fixed)

      usleep( USEC * ( 1 + rand() % 4 ) ); // random sleep for .1/.2/.3/.4 sec

      if( i == MAX_COL ) break; // reaching end, no need to erase last symbol

      PutChar( row, i, ' ', sem );      // or erase it
   }
   exit( (int)symbol ); // return my symbol info for parent to know who reached
}
