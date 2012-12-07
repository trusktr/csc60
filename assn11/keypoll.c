// keypoll.c
// AlphaDerby - Shared Memory (TwoTermsPick)
//
// can be compiled by itself without include common.h
// just return character (ASCII value) of key pressed
// set stdin echo off and non-block, check if key pressed,
// if so, then read it in and return

#include <stdio.h>
#include <unistd.h>
#include <string.h>     // strlen()
#include <termios.h>
#include <sys/types.h>
#include <sys/time.h>

// set stdin non-block or return to default block (1 on, 0 off)
void NonBlock(int on)
{
   struct termios tty_attr;
 
   tcgetattr(0, &tty_attr);              // get stdin attributes
 
   if ( on )                             // if given on (1)
   {
// turn off canonical (buffering) & echo modes
      tty_attr.c_lflag &= ~(ICANON | ECHO);
      tty_attr.c_cc[VMIN] = 1;           // just 1 char arrived will do
   }
   else
   {
      tty_attr.c_lflag |= ICANON & ECHO; // turn modes back on
   }
   tcsetattr(0, TCSANOW, &tty_attr);     // set stdin attributes
}
// return 1 if key pressed (before this called, char latched)
int KeyHit()
{
   struct timeval tv; // time value of poll
   fd_set fds;        // a set of fd (we use 1)

   tv.tv_sec = 0;     // cause no wait, poll time 0 whole sec
   tv.tv_usec = 0;    // cause no wait, poll time 0 microsec

   FD_ZERO(&fds);     // clear fd set
   FD_SET(0, &fds);   // 0 is STDIN_FILENO
   select(1, &fds, 0, 0, &tv); // 1 channel, two 0 (null)

   return FD_ISSET(0, &fds);   // test stdin (file # 0)
}

char main()
{
   int i, sec_left = 5;  // give player secs to pick letter
   char pick, str[100];

   srand(getpid());   // seed rand for random pick if user not pick in secs

   sprintf(str, "%c[2J", 27);
   write( 1, str, strlen(str) );
   sprintf( str, "%c[2;1H%s", 27, "Pick a winner A~Z " );
   write( 1, str, strlen(str) );

   NonBlock(1); // turn on non-block stdin

   while(1)
   {
      if( KeyHit() ) // check if key pressed
      {
         read(0, &pick, 1);  // read key-in as a char

// if not uppercase, reduce ASCII value to uppercase range
         if ( pick >= 'a' && pick <= 'z' ) pick -= ( 'a' - 'A' );

         write(1, "You've picked: ", 15);  // msg to show
         write(1, &pick, 1);                 // display pick
         write(1, " ", 2);                   // display " "

         sleep(1);                           // give time to let see msg

         if ( pick >= 'A' && pick <= 'Z' ) return pick; // good range
      }
      sprintf(str, "%c[4;1H%2d seconds! ", 27, sec_left);
      write(1, str, strlen(str));

      if( sec_left == 0 )
      {
         pick = rand()%26 + 'A';

         sprintf(str, "%c[4;13HI've picked for you: ", 27); // row 4, col 13
         write(1, str, strlen(str));

         for( i=0; i<5; i++ ) // flash random pick
         {
            usleep(250000);

            sprintf(str, "%c[4;34H  ", 27); // erase row 4, col 34
            write(1, str, strlen(str));

            usleep(250000);

            sprintf(str, "%c[4;34H%c ", 27, pick);
            write(1, str, strlen(str));
         }
         sleep(1);                                // give time to let see msg
         return pick;
      }
      sleep(1);
      sec_left--;
   }
   NonBlock(0);
}
