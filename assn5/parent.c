/*
 * Joe Pea -- Assignment 5 -- Weide Change
 */



#include <stdio.h> // input/output functions: printf()
/*#include <string.h> // string functions: strcpy() http://www.cplusplus.com/reference/clibrary/cstring/*/
/*#include <stdint.h> // provides constant-width integers (cross-platform)*/
#include <stdlib.h> // sprintf(), exit(), malloc(), free()
#include <time.h> // to get the time.
#include <unistd.h> // usleep(), 
#include <signal.h> // to use SIGKILL
#include <stdbool.h> // allows "bool" type and "true" and "false" macros.

#define MAX_ROUNDS 10



int main(int argc, char *args[]) {
	
	if (argc != 3 ) {
		printf("Usage: %s arg1 arg2\n", args[0]);
		exit(1);
	}

	int i,j,k; //counters
	unsigned int numberOfChildren = atoi(args[1]);
	unsigned int minFinalSum = atoi(args[2]);
	char minFinalSum_str[4];
	char outgoingPipe_str[3];
	char incomingPipe_str[3];
	char childNumber_str[3];
	pid_t wait_pid, exit_status;
	int randNumber = 0;
	int sum;
	int complete_count = 0;
	
	if (numberOfChildren < 1 || minFinalSum < 1 || numberOfChildren > 10 || minFinalSum > 100) {
		printf("Requirement: the arguments must be greater than 0. First arg no greater than 10. Second arg no greater than 100.\n");
		exit(2);
	}
	
	pid_t fork_pids[numberOfChildren]; // this is easier than declaring a global then using malloc or something.
	int* outgoingPipes[numberOfChildren]; // array of outgoing pipes.
	int* incomingPipes[numberOfChildren]; // array of incoming pipes.
	bool completed[numberOfChildren]; for (i=0; i<numberOfChildren; i++) completed[i] = false;
	
	/*
	 * Create the three children processes.
	 */
	for (i=0; i<numberOfChildren; i++) {
		outgoingPipes[i] = (int*)malloc( sizeof(int) * 2 ); // make the pipes.
		if ( pipe(outgoingPipes[i]) == -1 ) {
			printf("Error making pipe for child %i. \n", i);
			exit(3);
		}
		incomingPipes[i] = (int*)malloc( sizeof(int) * 2 ); // make the pipes.
		if ( pipe(incomingPipes[i]) == -1 ) {
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
		else if (fork_pids[i] == 0) { // 0 means we're in the child process.
			sprintf(minFinalSum_str, "%u", minFinalSum);
			sprintf(outgoingPipe_str, "%u", outgoingPipes[i][0]);
			sprintf(incomingPipe_str, "%u", incomingPipes[i][1]);
			sprintf(childNumber_str, "%u", i);
			execl("./child", outgoingPipe_str, incomingPipe_str, minFinalSum_str, 0 );
		}
		else { // any other number means we're in the parent process.
		}
	}
	
	
	printf(" -- We've got three children:\n");
	for (i=0; i<sizeof(fork_pids)/sizeof(pid_t); i++) {
		printf("   - PID# %u\n", fork_pids[i]);
	}
	printf(" \n");
	sleep(1);
	
	
	/*Let's initiate randomness based on the current time. */
	srand( (unsigned int)time(NULL) );
	
	while (complete_count < numberOfChildren) {
		int j=0;
		for (j=0; j<numberOfChildren; j++) {
			// bonus: skip this child if already done
			if (completed[j] == true) continue;
			
			/* Seed random randomly to be more random */
			for (k=0; k<rand()%100+50; k++) srand( rand() );
			randNumber = rand() % 10;
			
			usleep(100000);
			write( outgoingPipes[j][1], &randNumber, sizeof(int) );
			printf("Parent sent child #%i the number %i.\n", j, randNumber);
			
			// bonus: read another pipe to get the current sum of the child, if the
			// sum reaches the goal, set this child done, incr completion count
			read( incomingPipes[j][0], &sum, sizeof(int) );
			printf("Child #%u (PID %u) now has a sum of %i.\n", j, fork_pids[j], sum);
			if (sum > minFinalSum) {
				complete_count++;
				completed[j] = true;
			}
		}
	}
	
	/*
	 * Wait for the winner, then kill all the children.
	 */
	wait_pid = wait( &exit_status );
	for (i=0; i<numberOfChildren; i++) {
		if ( fork_pids[i] != wait_pid )
			kill(fork_pids[i], SIGKILL);
	}
	
	// exit code is only given 8-bit space, packed in 3rd byte
	exit_status <<= 16; exit_status >>= 24;
	printf("Parent here -- Child %i was the winner (sum == %i).\n", wait_pid, exit_status );
	
	for (i=0; i<numberOfChildren; i++) {
		free( outgoingPipes[i] );
		free( incomingPipes[i] );
	}
	
	return 0;
}














