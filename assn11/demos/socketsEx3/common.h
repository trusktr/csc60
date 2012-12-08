// common.h
// Example 3
// recv() is non-blocking!

#define SERVER_IP "130.86.67.249\0"  // titan is server, or below
// #define SERVER_IP "130.86.67.252\0"  // athena is server (choose one)

#define PORT_NUM 7890  // port # over 1023 are user (not root) ports

#define MAX_COL  60    // # of columns across
#define USEC  50000    // usleep(USEC), # of microsec sleep

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>    // write() needs this
#include <string.h>    // strlen() needs this

#include <fcntl.h>     // File control: O_CREAT flag (semaphores)
#include <semaphore.h> // sem_open(), etc.

#include <sys/types.h> // for shared mem
#include <sys/ipc.h>   // for shared mem
#include <sys/shm.h>   // for shared mem

#include <netdb.h>      // for sockets, struct sockaddr_in, ...
#include <sys/socket.h> // for sockets, socket calls

