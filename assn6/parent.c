/*
 * Joe Pea -- Assignment 6 -- Weide Change
 */



#include <stdio.h> // input/output functions: printf()
/*#include <string.h> // string functions: strcpy() http://www.cplusplus.com/reference/clibrary/cstring/*/
/*#include <stdint.h> // provides constant-width integers (cross-platform)*/
#include <stdlib.h> // sprintf(), exit(), malloc(), free()
#include <time.h> // to get the time.
#include <unistd.h> // usleep(), 
#include <signal.h> // SIGKILL, SIGINT, etc.
#include <stdbool.h> // allows "bool" type and "true" and "false" macros.

#define ping(x, y) kill(x, y)


unsigned int numberOfChildren;
int complete_count = 0;
pid_t *fork_pids_ptr;
bool *completed_ptr;
int **outgoingPipes_ptr;


void sigChildHandler();


int main(int argc, char *args[]) {
	
	if (argc != 3 ) {
		printf("Usage: %s arg1 arg2\n", args[0]);
		exit(1);
	}

	int i,j,k,l; // counters
	/*global*/ numberOfChildren = atoi(args[1]);
	unsigned int minFinalSum = atoi(args[2]);
	char minFinalSum_str[4];
	char outgoingPipe_str[3];
	int randNumber = 0;
	
	if (numberOfChildren < 1 || minFinalSum < 1 || numberOfChildren > 10 || minFinalSum > 100) {
		printf("Requirement: the arguments must be greater than 0. First arg no greater than 10. Second arg no greater than 100.\n");
		exit(2);
	}
	
	pid_t fork_pids[numberOfChildren]; // this is easier than declaring a global then using malloc or something.
	/*global*/ fork_pids_ptr = fork_pids;
	int* outgoingPipes[numberOfChildren]; // array of outgoing pipes.
	/*global*/ outgoingPipes_ptr = outgoingPipes;
	bool completed[numberOfChildren]; for (i=0; i<numberOfChildren; i++) completed[i] = false;
	/*global*/ completed_ptr = completed;
	
	/*
	 * Create the three children processes.
	 */
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
		else if (fork_pids[i] == 0) { // 0 means we're in the child process.
			sprintf(minFinalSum_str, "%u", minFinalSum);
			sprintf(outgoingPipe_str, "%u", outgoingPipes[i][0]);
			execl("./child", outgoingPipe_str, minFinalSum_str, 0 );
		}
	}
	
	
	printf(" -- We have %u children:\n", numberOfChildren);
	for (i=0; i<sizeof(fork_pids)/sizeof(pid_t); i++) {
		printf("   - PID# %u\n", fork_pids[i]);
	}
	printf(" \n");
	sleep(1);
	
	signal(SIGCHLD, sigChildHandler);
	
	
	/*Let's initiate randomness based on the current time. */
	srand( (unsigned int)time(NULL) );
	
	while (true) {
		for (j=0; j<numberOfChildren; j++) {
			// bonus: skip this child if already done
			if (completed[j] == true) continue;
			
			/* Seed random randomly to be more random */
			for (k=0; k<rand()%100+50; k++) srand( rand() );
			randNumber = rand() % 10;
			
			usleep(100000);
			write( outgoingPipes[j][1], &randNumber, sizeof(int) );
			printf("Parent sent child #%i (PID %u) the number %i.\n", j, fork_pids[j], randNumber);
			
			ping(fork_pids[j], SIGINT);
		}
	}
	
	return 0;
}


void sigChildHandler() {
	// Handle SIGCHLD.
	pid_t wait_pid, exit_status; int l,i;
	static int complete_count = 0;
	
	wait_pid = wait( &exit_status );
	
	// exit code is only given 8-bit space, packed in 3rd byte
	exit_status <<= 16; exit_status >>= 24;
	
	for (l=0; l<numberOfChildren; l++) {
		if (fork_pids_ptr[l] == wait_pid) {
			printf("Parent here, Child #%u (PID %u) exited with a sum of %u. \n", l, wait_pid, exit_status );
			completed_ptr[l] = true;
			complete_count++;
			if (complete_count == numberOfChildren) {
				for (i=0; i<numberOfChildren; i++) {
					free( outgoingPipes_ptr[i] );
				}
				exit(0);
			}
			break;
		}
	}
}














