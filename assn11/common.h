// common.h
// AlphaDerby - Shared Memory (TwoTermsPick)

#define MAX_COL  40    // # of columns across
#define USEC  50000    // usleep(USEC), # of microsec sleep

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>    // write() needs this
#include <string.h>    // strlen() needs this

#include <fcntl.h>     // O_CREAT flag (semaphores)
#include <semaphore.h> // sem_open(), etc.

#include <sys/types.h> // for shared mem
#include <sys/ipc.h>   // for shared mem
#include <sys/shm.h>   // for shared mem

void PutChar(int, int, char, sem_t *);
void Flash();
void InitScr(sem_t *);

// each letter symbol process needs one below
typedef struct
{
   char symbol;              // 'A' ~ 'Z'
   int row, col;             // on which row, proc itself updates col
} info_t;

