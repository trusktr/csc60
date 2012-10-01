/*
Joe Pea
Sacramento State
CSc 60 Introduction to System Programming in UNIX
Assignment #3

------------------------------------------------------------------------
After understanding how C macro definitions (C Preprocessor),
the "?" conditional operator, and bitwise operators work.
Write a C program that can demonstrate the use "?" and macros
(at ideal places) to perform the following work:

Use systems calls read() and write() for all input and output.
(Writing out a newline character or "fflush( stdout )" will be
handy at times when showing a prompted message not newline-terminated.)

Prompt with a message (write) for keyboard input first, then have
your last name entered (read) into a character array.

Part 1.
Display (using system call write()) the bits (with symbols 0 and 1)
from each byte in your last name, e.g., if "Chang" is entered:
C  0 1 0 0 0 0 1 1
h  0 1 1 0 1 0 0 0
a  0 1 1 0 0 0 0 1
n  0 1 1 0 1 1 1 0
g  0 1 1 0 0 1 1 1
(Separate symbols with spaces and one character byte per line.)
To do the above, nested loops are needed, the outer loop can go
through each character, and inner loop each bit in that character.

Part 2.
Use the first character from the entered name as the mask-in
value to mask all other characters and display them out like
that in part 1.

Part 3.
Use the first character from the entered name as the mask-out
value to mask all other characters and display them out like
that in part 1.

Bonus Part 1. (1 extra point)
By shifting existing bits in each character, make each byte
the largest (unsigned) value from that character, and display
them out like that in the first part. e.g., from the original
"Chang" (shown result like in Part 1) into:
0 1 0 0 0 0 1 1 -> 1 1 0 0 0 0 0 0
0 1 1 0 1 0 0 0 -> 1 1 0 1 0 0 0 0
0 1 1 0 0 0 0 1 -> 1 1 0 0 0 0 1 0
0 1 1 0 1 1 1 0 -> 1 1 1 0 0 0 0 0
0 1 1 0 0 1 1 1 -> 1 1 1 0 0 0 0 0
(Only need to show the result/right side.)

Bonus Part 2. (1 extra point)
Typical programming-contest problems would ask you to find
the size of the largest group of 0's encountered during the
listing of all the bits; also, the 1's; also, the total number
of bit groups (consecutive 0's and consecutive 1's) encountered.
Can your code answer these questions (applied to the bit sequence
of the original name input)? 

You may want to organize your code to have a subroutine that
can be called by each part to display bits from a given string.

------------------------------------------------------------------------
Turn-in
Turn in your source code the same way as you were required to do for
the other programming assignments. E-mailing will not be accepted, no
late turn-ins.
*/




#include <stdio.h> // input/output functions.
#include <string.h> // string functions
#include <stdint.h> // provides constant width integers (cross-platform)
#include <stdlib.h> // sprintf(),

/*The use of a macro to eliminate some duplicate code.*/
#define __GOOD_STUFF(__maskDeclaration, __bitSetter) \
	for (i=0; i<strlen(lastName); i++) { \
		bit = 0; \
		__maskDeclaration; \
		write(1, "\n", 1); \
		write(1, " ", 1); write(1, (char[]){lastName[i]}, 1); write(1, " ", 1); \
		for (j=7; j>=0; j--) { \
			bit = __bitSetter << (7-j); \
			bit = bit >> 7; \
			char ch[3]; /* 8 bit integers are at most 3 characters long in decimal form.*/ \
			sprintf(ch, "%u", (int)bit); \
			write(1, " ", 1); write(1, ch, 1); write(1, " ", 1); \
		} \
	} \
	write(1, "\n", 1);



int main(void) {
	int i = 0;
	int j = 0;
	char lastName[20];
	int length = 0;
	uint8_t bit = 0; // cross-platform constant width int of 8 bits... My computer is 64bit... School's are 32 bit... My program won't work on school computer otherwise.
	uint8_t masked;

	write(1, "1. Enter your last name: ", 25 ); // prompt to enter
	
	length = read(0, lastName, 20 );
	length--; // unaccount for the return
	
	lastName[length] = 0; // add the null terminator
	
	char firstLetter = lastName[0];
	
// PART 1 --------------------------------------------------
	__GOOD_STUFF( masked = 0, ((uint8_t)lastName[i]) )
	
// PART 2 --------------------------------------------------
	__GOOD_STUFF( masked = ((uint8_t)firstLetter) | ((uint8_t)lastName[i]), masked )
	
// PART 3 --------------------------------------------------
	__GOOD_STUFF( masked = ((uint8_t)firstLetter) & ((uint8_t)lastName[i]), masked )
	
	return 0;
}















