// parent.c
// AlphaDerby

#include "common.h"

char debug[200];

// when child has reached and exits, parent can do these things:
void ChildExit()
{
   static int place = 0;    // count 1st place, 2nd place, etc. (static var)
   char symbol, str[20];
   int pid, exit_code;

   pid = wait( &exit_code );   // perform a wait-fetch, shouldn't be blocked

sprintf(debug, "%s%u%s", "echo '-- debug parent -- Child Exited: ", pid, "'  > /dev/pts/2");
system(debug);
   symbol = (char) ( (exit_code << 16 ) >> 24 ); // get that "byte"

   place++;                           // upcount next winner's place
   printf( "%d-%c ", place, symbol ); // printf buffers this until later (below)

   if( place == 26 ) // all 26 letters have finished, we end
   {
      Flash();                          // flash screen
      sprintf( str, "\033[28;1H", 27 ); // put cursor low, on row 28, col 1
      write( 1, str, strlen(str) );     // make it happen
      printf("\n");                     // this equates "fflush(stdout);"
      exit(0);                          // parent can end
   }
}

int main()
{
   char symbol, str[30]; // each child's letter symbol, a general-purposed str
   sem_t *sem;           // the video semaphore that the parent needs to open

   signal( SIGCHLD, ChildExit ); // register the signal handler above to exec when child exits

   sprintf( str, "%u", getpid() ); // use my pid as key str to open a semaphore
   sem = sem_open( str, O_CREAT, S_IRWXU, 1 ); // a mutex type for video access
   if( sem == SEM_FAILED )
   {
      perror("Error making semaphore: ");
      return 0;
   }

   InitScr( sem );

   for( symbol='A'; symbol<='Z'; symbol++ ) // symbol goes thru 'A' to 'Z'
   {
      switch( fork() ) // fork 26 children
      {
         case -1: perror("Unable to fork child: "); return; // something went wrong
         case 0:                          // child process here
            sprintf( str, "%c", symbol ); // give child its symbol knowledge
            execl( "./child", "./child", str, 0 ); return; // argv[1] is str
      }
   }
   while(1) pause(); // hang for signal to kick in 26 times
}

