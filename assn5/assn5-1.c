/*
 * Joe Pea -- Assignment 4 -- Weide Change
 */



#include <stdio.h> // input/output functions.
/*#include <string.h> // string functions*/
/*#include <stdint.h> // provides constant-width integers (cross-platform)*/
#include <stdlib.h> // sprintf(),
#include <time.h> // to get the time.
#include <unistd.h> // usleep
#include <signal.h> // to use SIGKILL

#define MAX_ROUNDS 10



// globals are usually not recommended, but this is just one-man quick practice...
unsigned int numberOfChildren;
unsigned int minFinalSum;
unsigned int sum = 0;


void MyChild(int, int**);


int main(int argc, char *args[]) {
	
	if (argc != 3 ) {
		printf("Usage: %s arg1 arg2\n", args[0]);
		exit(1);
	}

	numberOfChildren = atoi(args[1]);
	minFinalSum = atoi(args[2]);
	
	if (numberOfChildren < 1 || minFinalSum < 1 || numberOfChildren > 10 || minFinalSum > 100) {
		printf("Requirement: the arguments must be greater than 0. First arg no greater than 10. Second arg no greater than 100.\n");
		exit(2);
	}
	
	pid_t fork_pids[numberOfChildren]; // this is easier than declaring a global then using malloc or something.
	pid_t wait_pid, exit_status;
	int randNumber = 0;
	int* outgoingPipes[numberOfChildren]; // array of pipes.
	
	/*
	 * Create the three children processes.
	 */
	unsigned int i=0;
	for (i=0; i<numberOfChildren; i++) {
		outgoingPipes[i] = (int*)malloc( sizeof(int) * 2 ); // make the pipes.
		if ( pipe(outgoingPipes[i]) == -1 ) {
			printf("Error making pipe for child %i. \n", i);
			exit(3);
		}
	}
	for (i=0; i<numberOfChildren; i++) { // fork the children
		fork_pids[i] = fork();
		
		if (fork_pids[i] == -1) { // -1 means error forking.
			printf("Error forking. \n"); // out of memory?
			exit(4);
		}
		else if (fork_pids[i] == 0) { // 0 means child process.
			MyChild(i, outgoingPipes);
		}
	}
	
	/*Let's initiate randomness based on the current time. */
	srand( (unsigned int)time(NULL) );
	
	for (i=0; i<MAX_ROUNDS; i++) {
		int j=0;
		for (j=0; j<numberOfChildren; j++) {
			// bonus: skip this child if already done
			
			/* Seed random randomly to be more random */
			int k=0; for (k=0; k<rand()%100+50; k++) srand( rand() );
			randNumber = rand() % 10;
			
			printf("Parent sends child #%i the number %i.\n", j, randNumber);
			
			write( outgoingPipes[j][1], &randNumber, sizeof(int) );
			
			// bonus: read another pipe to get the current sum of the child, if the
			// sum reaches the goal, set this child done, incr completion count
			
			usleep(1000);
		}
	}
	
	/*
	 * Wait for the children to exit.
	 */
		for (i=0; i<numberOfChildren; i++) {
			wait_pid = wait( &exit_status );
			if ( fork_pids[i] != wait_pid )
				kill(fork_pids[i], SIGKILL);
		}
		
		// exit code is only given 8-bit space, packed in 3rd byte
		exit_status <<= 16; exit_status >>= 24;
		printf("Parent here (PID %i) -- Child %i was the winner (sum == %i).\n", getpid(), wait_pid, exit_status );
		
		for (i=0; i<numberOfChildren; i++) free( outgoingPipes[i] );
		return 0;
}

void MyChild(int i, int** pipes) {
	int randNum = 0; // will receive from parent through pipe.
	
	while( sum < minFinalSum ) {
		// get random number between 0 and 9 from pipe.
		read( pipes[i][0], &randNum, sizeof(int) ); // waits for the parent to write to the pipe:
		sum = sum + randNum; //increment sum by random number.
		printf("Child #%u (PID %u) receives the number %i. Sum = %u\n", i, getpid(), randNum, sum); //display child proc id and current sum

		// bonus part: write current sum to another pipe (parent reads)
		// can sleep for 1 second in bonus version
	}
	exit(sum);
}














