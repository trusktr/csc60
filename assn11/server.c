// server.c
// AlphaDerby - Shared Memory (TwoTermsPick)

#include "common.h"

void main()
{
   sem_t *sem_video;         // video/stdout mutex
   char *p, str[30];         // for general purposes
   int shmids[26];           // 26 shared info_t areas
   info_t *infos[26];        // 26 ptrs to info of each alphabet
   int count, places[26];    // count how many done, their places
   int *cols4client, shmid4client; // for player to view 26 col #'s
   char *pick;               // cols4client[26] represents this
   int *points;              // cols4client[27] represents this
   int i;

   sprintf(str, "%d", getpid());
   sem_video = sem_open( str, O_CREAT, 0600, 1 ); // mutex type
   if( sem_video == SEM_FAILED ) { perror("sem_open"); return; }

   for( i=0; i<26; i++ ) // each alphabet needs info_t
   {
// request/allocate shared memory, size is one info_t
      if( ( shmids[i] = shmget( getpid()+'A'+i, sizeof(info_t), IPC_CREAT | 0600 ) ) < 0 )
      { perror( "shmget: " ); return; }

// attach shared memory segment to local data
      if( ( p = shmat( shmids[i], 0, 0 ) ) == (char *) -1 )
      { perror( "shmat: " ); return; }

      infos[i] = (info_t *)p;
      infos[i]->symbol = 'A'+i; // prep info_t for alphabet
      infos[i]->row = i+1;
   }

// for player client, 26 cols + pick + points
   if( ( shmid4client = shmget( getpid()+1, sizeof(int[28]), IPC_CREAT | 0600 ) ) < 0 )
   { perror( "shmget #2: " ); return; }

// attach shared memory segment to local data
   if( ( p = shmat( shmid4client, 0, 0 ) ) == (char *) -1 )
   { perror( "shmat: " ); return; }

   cols4client = (int *)p; // an array of int, 26 col #'s // size: 28
   
   // QUESTION: How do we know that the items in cols4client[] are all zero?
   // I'm wondering because in ./client.c we assume that items cols[] are all zero in order to compare to olds[].

// let client pick winner
   pick = (char *)&cols4client[26];         // last 2 allocated int spaces
   points = &cols4client[27];       // are used for bidding/scoring
   *pick = *points = 0;             // not picked and no points won

   printf( "\nFrom another shell, run client and pick winner: './client %d' ",
           shmid4client );
   fflush(stdout);

   while( (int)*pick == 0 ) usleep(500000); // .5 sec check-wait loop

	// QUESTION: How do we know that, from the above line to the below line, the client isn't done setting *pick in memory?

   printf( "\n\n   >>>>> The client picked %c <<<<< ", *pick );
   fflush(stdout);
   sleep(2);

// race starts from here
   InitScr( sem_video );

   for( i=0; i<26; i++ )
   {
      switch( fork() )
      {
         case -1: perror("fork"); return;
         case 0:
            sprintf( str, "%d", shmids[i] );
            execl( "./alphabet", "./alphabet", str, 0 ); return;
      }
   }
// each places[?] will be set when proc reaches goal
   for( i=0; i<26; i++ ) places[i] = 0; // not set
   count = 0;
   while( count < 26 ) // loop until all accounted for
   {
      for( i=0; i<26; i++ ) // check all proc status
      {
// this is for player client to view
         cols4client[i] = infos[i]->col;

// when proc reaches goal and 1st time we see this (places[?]==0)
         if( places[i] == 0  && infos[i]->col == MAX_COL )
         {
            count++;           // this is also overall placement
            places[i] = count; // marked its placement
            
            // QUESTION: What if two letters finish almost at the same time, will server mistake their positions?

// 1st 5 will get * shown in its placement
            if( count <= 5 ) sprintf( str, "%c[%d;%dH <-- %d ***",
                     27, i+1, MAX_COL+1, count );
            else sprintf( str, "%c[%d;%dH <-- %d",
                     27, i+1, MAX_COL+1, count );

            sem_wait(sem_video);
            write( 1, str, strlen(str) );   // make it happen
            sem_post(sem_video);

// check if client's pick wins
            if( 'A'+i == *pick ) *points = 5 - count + 1;

         } // end if
      } // end for( symbol=...
   } // end while( place < 26 )

   sleep(1); // don't flashing right away
   Flash();  // will also move cursor below things (to end cleanly)

   if( *points > 0 ) printf( "You've won %d points!\n\n", *points );
   else printf( "Better luck next time!\n\n ");

// close semaphore sem_video (use key str that opened it) and all
// shared mem requested via shmids[] and shmid4client
   sprintf( str, "%d", getpid() );   // use pid as key
   sem_unlink( str );                // close sem_video

   for( i=0; i<26; i++ ) shmctl( shmids[i], IPC_RMID, 0 );

   shmctl( shmid4client, IPC_RMID, 0 );

} // end main

