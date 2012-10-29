
/*
 * Joe Pea -- Assignment 7 -- Weide Chang 
 */


#include "common.h"


/*
 * Global variables.
 */
int msgQ; // the message queue to communicate with the parent.
msgQTransport messageBird;
int goalSum;


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
	int randNum = 0, sum = 0;
	/*global*/ msgQ = atoi(args[0]);
	/*global*/ goalSum = atoi(args[1]); // TODO Ask teacher why this has only 3 args unlike command line.
	if (msgQ < 0 || goalSum < 0) {
		printf("Requirement: the arguments must be greater than 0.\n");
		exit(2);
	}
	
	printf( "   Child here (%d), got message queue id %d.\n", getpid(), msgQ );
	
	/*
	 * Loop endlessly while waiting for messages.
	 */
	while( true ) {
	
		/* 
		 * Retrieve the random number sent from the parent when SIGINT.
		 */
		if ( -1 == msgrcv(msgQ, (void *)&messageBird, MSG_SIZE, getpid(), 0 ) ) { // the msg type is the child's pid.
			perror("Error reading from message queue.");
			exit(77);
		}
		randNum = messageBird.msg;
		sum = sum + randNum; //increment sum by random number.
		printf("    Child here (PID %u), number %u received, sum is now %u. \n", getpid(), randNum, sum);
		
		/*
		 * Notify parent of the new sum.
		 */
		messageBird.type = getppid();
		messageBird.msg = sum;
		if( -1 == msgsnd( msgQ, (void *)&messageBird, MSG_SIZE, 0 ) ) {
			printf( "Child: error sending message. \n" );
			exit(77);
		}

		/*
		 * Exit when the goal sum is reached.
		 */
		if (sum >= goalSum) {
			exit(sum);
		}
		
		usleep(100000);
	}
}








