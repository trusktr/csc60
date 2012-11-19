//***************************************************************************
// common.c - Joe Pea - Assignment 8 - CSC 60 - Weide Chang
// common routine for both parent.c and child.c
// skeleton of CR source code
//***************************************************************************

#include "common.h"   // sem_t and other stuff below

//***************************************************************************
// common routine for both parent and child code
void PutChar(int row, int col, char ch, sem_t *sem_video)
{
   char str[20];

   sem_wait( sem_video );  // if passing semaphore, ENTER guarded CRITICAL SECTION

   sprintf(str, "%c[%d;%dH", 27, row, col); // move cursor to row-col
   write(1, str, strlen(str));
   write(1, &ch, 1);                        // write out ch
   sprintf(str, "%c[H", 27);                // move cursor to home pos
   write(1, str, strlen(str));

   sem_post( sem_video );  // EXIT guarded CRITICAL SECTION
}

