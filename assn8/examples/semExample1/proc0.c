// POSIX Semaphores, Example, between Processes
// proc0.c
// gcc -lrt proc0.c -o proc0

// proc0 request a semaphore to be also used by proc1
// to guard the access of writing to stdout, in order
// to avoid racing condition

#include <stdio.h>      // read() write() printf()
#include <unistd.h>     // execl()
#include <string.h>     // strlen()
#include <fcntl.h>      // O_xxx flags
#include <sys/stat.h>   // SEM_FAILED and more constants
#include <semaphore.h>  // sem_open() sem_wait() sem_post() sem_getvalue()

void main() // use self pid as semaphore key
{
   int i, value, count;
   sem_t *sem;    // guarding access to stdout
   char str[100]; // 100 char should be enough
   char pid_str[8]; // use self pid as semaphore key

// prep semaphore, start value 1, make it like a mutex
// name of sem is also coded in proc1.c to obtain access
   sprintf(pid_str, "%d", getpid());            // pid as semaphore key
   sem = sem_open( pid_str, O_CREAT, 0600, 1 ); // set count 1

   if(sem == SEM_FAILED) // erred in sys call sem_open
   {
      perror("proc0 sem_open: ");
      return;
   }

   switch( fork() ) // child is proc1.c, needs the same sem key to use it
   {
      case -1: perror("fork: "); return;
      case 0: execl( "./proc1", "./proc1", 0 );
   }

   sem_getvalue( sem, &value ); // to obtain/verify initial count value
   printf("proc0: init sem starting count value (%d)...\n", value);

   count = 1;
   while(1)
   {
      sem_wait(sem); // may block if sem value is 0

      for( i = 0; i < count; i++ ) write( 1, "0", 1 ); // put some spaces afront
      write( 1, "\n", 1 );

      sem_post(sem); // +1 to value, so Child can pass sem_wait

      count++;
      if( count == 81 ) count = 1;
//      usleep(10000); // slow down at .01 sec interval to see conflicts
   }
// if not removed, same filename remains in /dev/shm/sem.<pid>
// and future code using the same name may not work correctly
   sem_unlink( pid_str );
}
