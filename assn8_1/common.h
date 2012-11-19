//***************************************************************************
// common.h - Joe Pea - Assignment 8 - CSC 60 - Weide Chang
// skeleton of CR source code
//***************************************************************************
//
// CSc 60 IPC: Semaphores (2-CR: Chaser and Runner)
// parent.c and child.c include this

#ifndef __COMMON_H__
	#define __COMMON_H__

	#define MAX_ROW  8  // game space height
	#define MAX_COL  16  // game space width
	#define USEC 100000 // minimum .1 sec sleep

	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>    // write() needs this
	#include <string.h>    // strlen() needs this
	#include <fcntl.h>     // O_xxx flags
	#include <sys/stat.h>  // SEM_FAILED and more constants
	#include <semaphore.h>
	#include <signal.h>

	// prototype this one:
	void PutChar(int, int, char, sem_t *);

#endif
