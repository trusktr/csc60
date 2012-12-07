// alphabet.c
// AlphaDerby - Shared Memory (TwoTermsPick)

#include "common.h"

void main(int argc, char *argv[]) // shared memory ID passed from server
{
   int shmid;
   info_t *my_info;
   sem_t *sem_video;
   char *p, str[30];
  
   shmid = atoi( argv[1] ); // needs to know shmid for my_info

   if( ( p = shmat( shmid, 0, 0 ) ) == (char *) -1 ) // attach to p
   {
      perror( "alphabet shmat: " );
      return;
   }
   my_info = (info_t *)p; // strangely got to go around via p (char *)

   sprintf(str, "%d", getppid()); // conveniently use server PID as key str
   sem_video = sem_open( str, O_CREAT );
   if(sem_video == SEM_FAILED) { perror("alphabet sem_open: "); return; }

   srand( getpid() );   // seed rand()

   for( my_info->col=1; my_info->col<=MAX_COL; my_info->col++ )
   {
      PutChar( my_info->row, my_info->col, my_info->symbol, sem_video );

      usleep( USEC * ( 1 + rand() % 4 ) ); // 1~4 portions of USEC

      if( my_info->col == MAX_COL ) break; // no need to erase last

      PutChar( my_info->row, my_info->col, ' ', sem_video ); // erase
   }
}
