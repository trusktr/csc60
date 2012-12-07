// client.c
// AlphaDerby - Shared Memory (TwoTermsPick)

#include "common.h"

void main(int argc, char *argv[])
{
   sem_t *sem_video;
   char *p, str[30];         // for general purposes
   int *cols, shmid;         // shared area, shared mem ID
   int count, places[26];    // count how many done, their places
   int olds[26];             // for compare-then-update
   int pid, exit_code;       // for keypoll
   char *pick;               // for keypoll
   int *points;              // for keypoll
   int i;

   shmid = atoi( argv[1] );

// attach shared memory segment to local data
   if( ( p = shmat( shmid, 0, 0 ) ) == (char *) -1 )
   { perror( "shmat: " ); return; }

   cols = (int *)p;
   pick = (char *)&cols[26];         // 27th for pick feedback
   points = &cols[27];               // 28th for getting points

   sprintf(str, "%d", getpid());
   sem_video = sem_open( str, O_CREAT, 0600, 1 ); // mutex type
   if( sem_video == SEM_FAILED ) { perror("sem_open"); return; }

// get winner pick from keypoll
   switch( fork() )
   {
      case -1: perror("fork"); return;
      case 0: execl( "./keypoll", "./keypoll", 0 ); return;
   }
   pid = wait( &exit_code ); // wait for keypoll to return

// let server know what I pick
   *pick = (char)(( exit_code << 16 ) >> 24 );

// start viewing race
   InitScr( sem_video );

// show on right which letter player picked
   sprintf( str, "%c[%d;%dH <-------- GOOD LUCK", 27, *pick-'A'+1, MAX_COL+1 );
   write( 1, str, strlen(str) );     // make it happen

// each places[?] will be set when proc reaches goal
   for( i=0; i<26; i++ ) places[i] = 0; // not set

// keep old to compare if updates occur
   for( i=0; i<26; i++ ) olds[i] = 0; // not set

   count = 0;
   
   // QUESTION: Is this loop randomly concurrent with the while loop in ./server.c?
   
   while( count < 26 ) // loop until all accounted for
   {
      for( i=0; i<26; i++ ) // check all proc status
      {
         // QUESTION: Why do we assume that items in cols[] are all zero initially? See question in ./server.c
         // regarding cols4client.
         if( cols[i] != olds[i] ) // new col, change occurred
         {
         	// QUESTION: why (olds[i] > 0)? I tried (olds[i] >= 0) and the result is the same.
            if( olds[i] >= 0 ) // beginning, col 0, don't do erasure
               PutChar( i+1, olds[i], ' ', sem_video );
            PutChar( i+1, cols[i], 'A'+i, sem_video ); // show new
            olds[i] = cols[i];                   // new becomes old

// when proc reaches goal and 1st time we see this (marks[?]==0)
            if( places[i] == 0  && cols[i] == MAX_COL )
            {
               count++;           // this is also overall placement
               places[i] = count; // marked its placement

// 1st 5 will get *** shown in its placement
               if( count <= 5 ) sprintf( str, "%c[%d;%dH <-- %d ***",
                        27, i+1, MAX_COL+1, count );
               else sprintf( str, "%c[%d;%dH <-- %d",
                        27, i+1, MAX_COL+1, count );

               write( 1, str, strlen(str) );   // make it happen

            } // end if( places...
         } // end if( cols...
      } // end for( ...
   } // end while( place < 26 )

   sleep(1); // not flashing too right away
   Flash();  // will also move cursor below things (to end cleanly)

// print scoring given by server
   printf( "Points won: %d\n\n", *points );

// close semaphore sem_video (use key str that opened it) and all
// shared mem requested via shmids[]
   sprintf(str, "%d", getpid());   // use pid as key
   sem_unlink( str );              // close sem_video

} // end main

