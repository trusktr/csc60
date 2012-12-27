// common.h
// AlphaDerby - Shared Memory (TwoTermsPick)

#define SERVER_IP "localhost\0" // local server.
// #define SERVER_IP "130.86.67.249\0"  // titan is server, or below
// #define SERVER_IP "130.86.67.252\0"  // athena is server (choose one)

#define LOCAL_PORT_NUM 7890  // port # over 1023 are user (not root) ports
#define LIVE_PORT_NUM 7891  // port # over 1023 are user (not root) ports

#define MAX_COL  40    // # of columns across
#define USEC  50000    // usleep(USEC), # of microsec sleep
#define MAX_CLIENTS 100
#define CONNECTION_TIME 10

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>    // write() needs this
#include <string.h>    // strlen() needs this

#include <fcntl.h>     // O_CREAT flag (semaphores)
#include <semaphore.h> // sem_open(), etc.

#include <sys/types.h> // for shared mem
#include <sys/ipc.h>   // for shared mem
#include <sys/shm.h>   // for shared mem

#include <netdb.h>      // for sockets, struct sockaddr_in, ...
#include <sys/socket.h> // for sockets, socket calls

#include <pthread.h>	// for threads.

void PutChar(int, int, char, sem_t *);
void Flash();
void InitScr(sem_t *);

// each letter symbol process needs one below
typedef struct
{
   char symbol;              // 'A' ~ 'Z'
   int row, col;             // on which row, proc itself updates col
} info_t;

