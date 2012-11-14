// common.h
// AlphaDerby

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>    // write() needs this
#include <string.h>    // strlen() needs this

#include <fcntl.h>     // O_CREAT flag (semaphores)
#include <semaphore.h> // sem_open(), etc.

#include <signal.h>

#define MAX_COL 80     // # of columns across
#define USEC 50000    // usleep(USEC) sleeps .1 sec

void PutChar(int, int, char, sem_t *);
void Flash();
void InitScr(sem_t *);

