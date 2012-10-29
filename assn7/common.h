
/*
 * Joe Pea -- Assignment 7 -- Weide Chang 
 */


#ifndef __COMMON_H__
	#define __COMMON_H__

	#include <stdio.h> // input/output functions: printf()
	/*#include <string.h> // string functions: strcpy() http://www.cplusplus.com/reference/clibrary/cstring/*/
	/*#include <stdint.h> // provides constant-width integers (cross-platform)*/
	#include <stdlib.h> // sprintf(), exit(), malloc(), free()
	#include <time.h> // to get the time.
	#include <unistd.h> // usleep(), 
	#include <signal.h> // SIGKILL, SIGINT, etc.
	#include <stdbool.h> // allows "bool" type and "true" and "false" macros.

	#include <sys/types.h> // goes along with sys/msg.h
	#include <sys/ipc.h> // goes along with sys/msg.h
	#include <sys/msg.h>   // msg flags, msgget/msgsnd/msgrcv/msgctl()

	#define ping(x, y) kill(x, y) // rename the kill function.

	typedef struct {
		long int type;       /* message type, must be > 0 */
		int msg; 			   /* message data */
	} msgQTransport;

	#define MSG_SIZE ( sizeof( msgQTransport ) - sizeof( long int ) )


#endif




