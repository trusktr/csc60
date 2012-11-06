// POSIX Semaphores, Example, between Processes
// main.c
// gcc -lrt main.c

#include <stdio.h>      // read() write() printf()
#include <unistd.h>     // execl()
#include <fcntl.h>      // O_xxx flags
#include <sys/stat.h>   // SEM_FAILED and more constants
#include <semaphore.h>  // sem_open() sem_wait() sem_post()

void Child(int pipe_in)
{
   int num;
   sem_t *sem; // guarding access to stdout
   char ppid_str[8]; // parent PID str as semaphore key

   sleep(2);   // get parent a head start

// get semaphore parent made
   sprintf(ppid_str, "%d", getppid()); // convert # to str
   sem = sem_open( ppid_str, O_CREAT );

   if(sem == SEM_FAILED)
   {
      perror("child sem_open: ");
      return;
   }

   while(1)
   {
      sem_wait(sem); // will block if value in sem is 0

      read(pipe_in, &num, sizeof(int));
      printf("   Child: read from pipe, num %d...\n", num);

      sem_post(sem); // post it (+1)

      sleep(1);
   }
}

void main()
{
   int pid, num, the_pipe[2];
   sem_t *sem;       // guarding access to stdout
   char pid_str[8];  // self PID str as semaphore key

// prep semaphore, start value 1, like a mutex
   sprintf(pid_str, "%d", getpid());            // convert pid to str
   sem = sem_open( pid_str, O_CREAT, 0600, 1 ); // pid_str as key

   if(sem == SEM_FAILED)
   {
      perror("parent sem_open: ");
      return;
   }

   if(-1 == pipe(the_pipe))
   {
      perror("pipe: ");
      return;
   }

   pid = fork();
   if(pid == -1)
   {
      perror("fork: ");
      return;
   }

   if(pid == 0)
   {
      Child(the_pipe[0]); // dangerously called
      return;             // better have this
   }

   srand(getpid());

   while(1)
   {
      sem_wait(sem); // with this "polite protocol" to pipe

      num = rand()%10;
      printf("Parent: write to pipe, num %d...\n", num);
      write(the_pipe[1], &num, sizeof(int));

      sem_post(sem); // OK, +1 for the child to pass sem_wait

      sleep(1);
   }
   close(the_pipe[0]); // good/clean practice
   close(the_pipe[1]); // good/clean practice

// if not removed, same filename remains: /dev/shm/sem.<pid>
// and future code using the same name may not work correctly
   sem_unlink( pid_str );
}
