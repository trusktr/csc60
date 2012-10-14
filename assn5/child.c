/*
 * Joe Pea -- Assignment 5 -- Weide Change
 */


#include <stdio.h>
#include <stdlib.h>



int main(int argc, char *args[]) {
	
	if (argc != 3 ) {
		printf("Usage: %s arg1 arg2 arg3\n", args[0]);
		exit(1);
	}

	int incomingPipeChannel = atoi(args[0]);
	int outgoingPipeChannel = atoi(args[1]);
	int minFinalSum = atoi(args[2]); // TODO Ask teacher why this has only 3 args unlike command line.
	int randNum = 0; // will receive from parent through pipe.
	int sum = 0;
	
	if (outgoingPipeChannel < 0 || incomingPipeChannel < 0 || minFinalSum < 0) {
		printf("Requirement: the arguments must be greater than 0.\n");
		exit(2);
	}
	
	while( sum < minFinalSum ) {
		// get random number between 0 and 9 from pipe.
		read( incomingPipeChannel, &randNum, sizeof(int) ); // waits for the parent to write to the pipe:
		sum = sum + randNum; //increment sum by random number.

		// bonus part: write current sum to another pipe (parent reads)
		// can sleep for 1 second in bonus version
		write( outgoingPipeChannel, &sum, sizeof(int) ); // tell the parent the new sum.
	}
	
	exit(sum);
}








