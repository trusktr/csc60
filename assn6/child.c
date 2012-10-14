

#include <stdio.h>
#include <stdlib.h>



int main(int argc, char *args[]) {

	system("echo ' -- #########################################'");
	
	if (argc != 4 ) {
		printf("Usage: %s arg1 arg2 arg3\n", args[0]);
		exit(1);
	}

	int childNumber = atoi(args[1]);
	int incomingPipeChannel = atoi(args[2]);
	int minFinalSum = atoi(args[3]);
	int randNum = 0; // will receive from parent through pipe.
	int sum = 0;
	
	if (childNumber < 0 || incomingPipeChannel < 0 || minFinalSum < 0) {
		printf("Requirement: the arguments must be greater than 0.\n");
		exit(2);
	}
	
	while( sum < minFinalSum ) {
		// get random number between 0 and 9 from pipe.
		read( incomingPipeChannel, &randNum, sizeof(int) ); // waits for the parent to write to the pipe:
		sum = sum + randNum; //increment sum by random number.
		write(1, "test", 4);
		printf("Child #%u (PID %u) receives the number %i. Sum = %u\n", childNumber, getpid(), randNum, sum); //display child proc id and current sum

		// bonus part: write current sum to another pipe (parent reads)
		// can sleep for 1 second in bonus version
	}
	
	exit(sum);
}








