// parent.c - Joe Pea - Assignment 8 - CSC 60 - Weide Chang
// CR: Chaser and Runner
// CSc 60 IPC: Semaphores

#include "common.h"

void Flash() // flashes display
{
   char str[20];

   sprintf(str, "%c[?5h", 27); // inverse video mode kicks in
   write(1, str, strlen(str));

   usleep( USEC );             // sleep/delay .1 sec

   sprintf(str, "%c[?5l", 27); // normal video mode back
   write(1, str, strlen(str));

   sprintf(str, "%c[?25h", 27); // cursor back on, off is [?25l
   write(1, str, strlen(str));
}

void InitScr(sem_t *sem_video) // draw out initial screen
{
   char str[20];
   int i;

   sprintf(str, "%c[2J", 27);   // clear screen
   write(1, str, strlen(str));

   sprintf(str, "%c[?25l", 27); // cursor off, on is [?25h
   write(1, str, strlen(str));

   for(i=1; i<=MAX_ROW; i++)    // rows 1..MAX_ROW
   {
      PutChar(i, MAX_COL+1, '|', sem_video); // last col
   }
   for(i=1; i<=MAX_COL+1; i++)  // bottom row, col 0..MAX_COL+1
   {
      PutChar(MAX_ROW+1, i, '-', sem_video); // bottom row
   }
}

void main() { // no need of given args, use self pid as semaphore key

   int c_row, c_col, r_row, r_col;
   int the_pipe[2];                 // 1 pipe for both children
   sem_t *sem_video, *sem_c, *sem_r;
   char str[100];                     // str for common usage
   char row_str[3], col_str[3], pipe_str[3], symbol_str[3]; // child's argv

system("echo ------------------------- > /dev/pts/5");
sprintf(str, "%s%u%s", "echo '-- debug parent -- PID: ", getpid(), "'  > /dev/pts/5");
system(str);

// prep video semaphore, start value 1, make it like a mutex
   sprintf( str, "%u", getpid() );        // build key str out of pid
   sem_video = sem_open( str, O_CREAT, S_IRWXU, 1 );

// prep Chaser semaphore, start value 0
   sprintf( str, "%u", 'C'+getpid() );    // key str out of pid + 'C'
   sem_c = sem_open( str, O_CREAT, S_IRWXU, 0 );

// prep Runner semaphore, start value 0
   sprintf( str, "%u", 'R'+getpid() );    // key str out of pid + 'R'
   sem_r = sem_open( str, O_CREAT, S_IRWXU, 0 );

   if(sem_video == SEM_FAILED) // erred in sys call sem_open
   {
      perror("Error opening video semaphore: ");
      return;
   }
   if(sem_c == SEM_FAILED) // erred in sys call sem_open
   {
      perror("Error opening chaser semaphore: ");
      return;
   }
   if(sem_r == SEM_FAILED) // erred in sys call sem_open
   {
      perror("Error opening runner semaphore: ");
      return;
   }

   if( -1 == pipe(the_pipe) ) // 1 pipe for both children
   {
      perror("Error making pipe: ");
      return;
   }

   InitScr( sem_video );           // starting screen

// prep argv for Chaser, starts at upper-left corner
   sprintf(row_str, "%u", 1);    // starting row pos, packing info into str
   sprintf(col_str, "%u", 1);    // starting col pos, packing info into str
   sprintf(pipe_str, "%u", the_pipe[1]);   // pipe end for it to write back
   sprintf(symbol_str, "%c", 'C'); // Chaser's char symbol

   switch( fork() )                // launch Chaser child
   {
      case -1: perror("Error forking chaser: "); return;   // error in fork
      case 0: execl( "./child", "./child",   // start child proc with argv
                 row_str, col_str, pipe_str, symbol_str, 0 );
   }

// prep argv for Runner, starts at lower-right corner
   sprintf(row_str, "%u", MAX_ROW);
   sprintf(col_str, "%u", MAX_COL);
//   sprintf(pipe_str, "%u", ...); // same pipe_str as above
   sprintf(symbol_str, "%c", 'R');

   switch( fork() )                // launch Runner child
   {
      case -1: perror("fork R: "); return;
      case 0: execl( "./child", "./child",
                 row_str, col_str, pipe_str, symbol_str, 0 );
   }

   int loopCount = 0;
   while(1) //  let them play forever
   {
   
sprintf(str, "%s%u%s", "echo '-- debug parent -- while iteration ", loopCount, ", position 1' > /dev/pts/5");
system(str);
      sem_post( sem_c ); // allow Chaser 1 play move (loop once, see child.c)

      read( the_pipe[0], &c_row, sizeof(int) ); // read pos info row from Chaser
sprintf(str, "%s%u%s", "echo '-- debug parent -- while iteration ", loopCount, ", position 2' > /dev/pts/5");
system(str);
      read( the_pipe[0], &c_col, sizeof(int) ); // read pos info col from Chaser

      if(c_row == r_row && c_col == r_col) break; // collision occurred?

      sem_post( sem_r ); // allow Runner 1 play move (loop once, see child.c)

      read( the_pipe[0], &r_row, sizeof(int) ); // read pos info from Runner
      read( the_pipe[0], &r_col, sizeof(int) ); // read pos info from Runner

      if(c_row == r_row && c_col == r_col) break; // collision occurred?
   }

   sem_wait(sem_video); // since children are still running, hold till end

   Flash();      // flash screen, restore cursor

   sprintf(str, "%c[%d;%dH", 27, MAX_ROW+3, 1);  // move cursor to bottom
   write(1, str, strlen(str));
   printf("\n*** Chaser got Runner! *** \n");         // OK to use printf since
   printf("Chaser at row %u col %u\n", c_row, c_col); // we're all done with
   printf("Runner at row %u col %u\n", r_row, r_col); // moving around symbols

// if not removed, same filename remains in /dev/shm/sem.<pid>
   sprintf( str, "%u", getpid() );     // build key str out of pid
   sem_unlink( str );
   sprintf( str, "%u", 'C'+getpid() ); // build key str out of pid+'C'
   sem_unlink( str );
   sprintf( str, "%u", 'R'+getpid() ); // build key str out of pid+'R'
   sem_unlink( str );

   kill(0, SIGHUP); // shutdown all processes, include this
}














