
/*
 * Joe Pea -- Assignment 6 -- Weide Chang 
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>


/*
 * Global variables.
 */
int incomingPipeChannel;
int goalSum;


/*
 * Function prototypes.
 */
void sigIntHandler();


/*
 * Entry point.
 */
int main(int argc, char *args[]) {
	
	/*
	 * Main initialization.
	 */
	if (argc != 2 ) {
		printf("Usage: %s arg1 arg2 \n", args[0]);
		exit(1);
	}
	/*global*/ incomingPipeChannel = atoi(args[0]);
	/*global*/ goalSum = atoi(args[1]); // TODO Ask teacher why this has only 3 args unlike command line.
	if (incomingPipeChannel < 0 || goalSum < 0) {
		printf("Requirement: the arguments must be greater than 0.\n");
		exit(2);
	}
	
	/*
	 * Register sigIntHandler() as the handler for the SIGINT signal.
	 */
	signal(SIGINT, sigIntHandler);
	
	/*
	 * Loop endlessly while waiting for a signals from the parent.
	 */
	while( true ) {
		printf("Child here (PID %u), waiting for signal... \n", getpid());
		usleep(1000000);
	}
}


/*
 * The handler for the SIGINT signal.
 */
void sigIntHandler() {
	static int randNum = 0, sum = 0;
	
	/* 
	 * Retrieve the random number sent from the parent when SIGINT.
	 */
	read( incomingPipeChannel, &randNum, sizeof(int) );
	sum = sum + randNum; //increment sum by random number.
	printf("Child here (PID %u), number %u received, sum is now %u. \n", getpid(), randNum, sum);

	/*
	 * Exit when the goal sum is reached.
	 */
	if (sum >= goalSum) {
		exit(sum);
	}
}








