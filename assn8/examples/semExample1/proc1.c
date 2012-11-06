// POSIX Semaphores, Example, between Processes
// proc1.c
// gcc -lrt proc1.c -o proc1

// proc1 gets access to semaphore proc0 created and
// thus access stdout that's shared among them

#include <stdio.h>      // read() write() printf()
#include <unistd.h>     // execl()
#include <string.h>     // strlen()
#include <fcntl.h>      // O_xxx flags
#include <sys/stat.h>   // SEM_FAILED and more constants
#include <semaphore.h>  // sem_init() sem_wait() sem_post()

void main() // use parent pid as semaphore key
{
   int i, value, count;
   sem_t *sem; // guarding access to stdout
   char str[100];
   char ppid_str[8]; // build str from getppid()

// with the right key, can get access already created (by proc0)
   sprintf(ppid_str, "%d", getppid());
   sem = sem_open( ppid_str, O_CREAT );

   if(sem == SEM_FAILED) // erred in sys call
   {
      perror("proc1 sem_open: ");
      return;
   }

   count = 1;
   while(1)
   {
      sem_wait(sem); // access control via sem wait call

      for( i = 0; i < count; i++ ) write( 1, "1", 1 );
      write( 1, "\n", 1 );

      sem_post(sem); // +1 to sem value (release access)

      count++;
      if( count == 81 ) count = 1;
//      usleep(10000); // slow it down at .01 sec to see conflicts
   }
}
