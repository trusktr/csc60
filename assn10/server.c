// server.c
// AlphaDerby - Shared Memory (Standard)

#include "common.h"

void main()
{
   sem_t *sem_video;          // video/stdout mutex
   char *p, str[30];          // for general purposes
   int shmids[26];            // 26 shared info_t areas
   info_t *infos[26];         // 26 ptrs to info of each alphabet
   int count, places[26];     // count how many done, their places
   int i;                     // for loops

   sprintf( str, "%d", getpid() ); // use my PID as key str
   sem_video = sem_open( str, O_CREAT, S_IRWXU, 1 ); // open semaphore, mutex type
   if( sem_video == SEM_FAILED ) { perror("sem_open"); return; }

   for( i=0; i<26; i++ ) // go thru 26 alphabets
   {
// request/allocate shared memory, size is one info_t
      if( ( shmids[ i ] = shmget( getpid()+'A'+i, sizeof( info_t ), IPC_CREAT | 0600 ) ) < 0 )
      {
         perror( "shmget: " );
         return;
      }
// attach shared memory segment to local data
      if( ( p = shmat( shmids[ i ], 0, 0 ) ) == (char *) -1 )
      {
         perror( "shmat: " );
         return;
      }
// prep info_t for alphabet
      infos[ i ] = (info_t *)p;  // addr of info_t is what p has
      infos[ i ]->symbol = 'A'+i;     // prep alphabet symbol, need casting??
      infos[ i ]->row = i+1;     // prep alphabet row # to race
   }

   InitScr( sem_video );

   for( i=0; i<26; i++ ) // go thru 26 alphabets
   {
      switch( fork() )
      {
         case -1: perror("fork"); return;
         case 0:
            // ... prep str with shmids[...] ...
            sprintf(str, "%i", shmids[i]);
            execl( "./alphabet", str, 0 );
      }
   }
// each places[?] will be set when proc reaches goal
   for( i=0; i<26; i++ ) places[ i ] = 0; // all not set
   count = 0;
   while( count < 26 ) // loop until all accounted for
   {
      for( i=0; i<26; i++ ) // check all proc status
      {
// when proc reaches goal and 1st time we see this (...[...]==0)
         if( places[ i ] == 0  && infos[ i ]->col == MAX_COL )
         {
            count++;                      // this is also overall placement
            places[ i ] = count; // marked its placement

// top 5 will get *** shown in its placement
            if( count <= 5 ) sprintf( str, "%c[%d;%dH <-- %d ***", 27, infos[i]->row, MAX_COL+2, count );
            else             sprintf( str, "%c[%d;%dH <-- %d", 27, infos[i]->row, MAX_COL+2, count );

            sem_wait( sem_video );
            write( 1, str, strlen(str) );   // make it happen
            sem_post( sem_video );

         } // end if
      } // end for( i=...
   } // end while( place < 26 )

// close semaphore sem_video (use key str that opened it) and all
// shared mem requested via shmids[]
   sprintf( str, "%d", getpid() );   // use pid as key
   sem_unlink( str );                // close sem_video

   for( i=0; i<26; i++ ) shmctl( shmids[ i ], IPC_RMID, 0 );

   sleep(1); // not flashing too right away
   Flash();  // will also move cursor below things (to end cleanly)

} // end main




