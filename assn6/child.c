




/* Joe Pea -- Assignment 6 -- Weide Chang */




#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>

int incomingPipeChannel;
int minFinalSum;

void sigIntHandler();

int main(int argc, char *args[]) {
	
	if (argc != 2 ) {
		printf("Usage: %s arg1 arg2 \n", args[0]);
		exit(1);
	}

	/*global*/ incomingPipeChannel = atoi(args[0]);
	/*global*/ minFinalSum = atoi(args[1]); // TODO Ask teacher why this has only 3 args unlike command line.
	
	if (incomingPipeChannel < 0 || minFinalSum < 0) {
		printf("Requirement: the arguments must be greater than 0.\n");
		exit(2);
	}
	
	signal(SIGINT, sigIntHandler);
	
	while( true ) {
		printf("Child here (PID %u), waiting for signal... \n", getpid());
		usleep(1000000);
	}
}


void sigIntHandler() {	//handle SIGINT
	static int randNum = 0, sum = 0;
	
	// get random number between 0 and 9 from pipe.
	read( incomingPipeChannel, &randNum, sizeof(int) ); // waits for the parent to write to the pipe:
	sum = sum + randNum; //increment sum by random number.
	printf("Child here (PID %u), my sum is now %u. \n", getpid(), sum);

	if (sum >= minFinalSum) {
		exit(sum);
	}
}








