
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

/*
 * Global variables.
 */
unsigned int numberOfChildren;
int complete_count = 0;
pid_t *fork_pids_ptr; // pointer to array of child PIDs.
bool *completed_ptr; // pointer to array of booleans representing each child's completion.
int **outgoingPipes_ptr; // pointer to array of pipes for sending a number to each child.

/*
 * Function prototypes.
 */
void sigChildHandler(); // handler for when a child exits and sends the SIGCHLD signal. Definition below.

/*
 * Entry point.
 */
int main(int argc, char *args[]) {
	
	/*
	 * Initialization for main()
	 */
	if (argc != 3 ) {
		printf("Usage: %s arg1 arg2\n", args[0]);
		exit(1);
	}
	/*global*/ numberOfChildren = atoi(args[1]);
	unsigned int goalSum = atoi(args[2]);
	char goalSum_str[4];
	if (numberOfChildren < 1 || goalSum < 1 || numberOfChildren > 10 || goalSum > 100) {
		printf("Requirement: the arguments must be greater than 0. First arg no greater than 10. Second arg no greater than 100.\n");
		exit(2);
	}
	int i,j,k,l; // counters
	int randNumber = 0;
	pid_t fork_pids[numberOfChildren]; // Array of child PIDs whose size is numberOfChildren.
	/*global*/ fork_pids_ptr = fork_pids;
	int* outgoingPipes[numberOfChildren]; // array of outgoing pipes.
	char outgoingPipe_str[3];
	/*global*/ outgoingPipes_ptr = outgoingPipes;
	bool completed[numberOfChildren]; for (i=0; i<numberOfChildren; i++) completed[i] = false;
	/*global*/ completed_ptr = completed;
	
	/*
	 * Create the pipes for each child..
	 */
	for (i=0; i<numberOfChildren; i++) {
		outgoingPipes[i] = (int*)malloc( sizeof(int) * 2 ); // make the pipes.
		if ( pipe(outgoingPipes[i]) == -1 ) {
			printf("Error making pipe for child %i. \n", i);
			exit(3);
		}
	}
	
	/*
	 * Fork each child process.
	 */
	for (i=0; i<numberOfChildren; i++) { // fork the children
		fork_pids[i] = fork();
		
		if (fork_pids[i] == -1) { // -1 means error forking.
			printf("Error forking. \n"); // out of memory?
			exit(4);
		}
		else if (fork_pids[i] == 0) { // 0 means we're in the child process.
			sprintf(goalSum_str, "%u", goalSum);
			sprintf(outgoingPipe_str, "%u", outgoingPipes[i][0]);
			
			/*
			 * Use execl() to make each fork replace itself with an new process: "./child"
			 */
			execl("./child", outgoingPipe_str, goalSum_str, 0 );
		}
	}
	
	/*
	 * Register sigChildHandler() as the handler for the SIGCHLD signal.
	 */
	signal(SIGCHLD, sigChildHandler);
	sleep(1);
	
	
	/*
	 * Let's initiate some randomness based on the current time.
	 */
	srand( (unsigned int)time(NULL) );
	
	/*
	 * The main loop for sending numbers to each child.
	 */
	while (true) {
		for (j=0; j<numberOfChildren; j++) { // for each child.
			
			/* 
			 * BONUS: skip this child if it has already exited.
			 */
			if (completed[j] == true) continue;
			
			usleep(100000);
			
			/*
			 * Seed random randomly to be more random.
			 */
			for (k=0; k<rand()%100+50; k++) srand( rand() );
			
			/*
			 * Send a random number to this child.
			 */
			randNumber = rand() % 10;
			write( outgoingPipes[j][1], &randNumber, sizeof(int) );
			printf("Parent here, sent child #%i (PID %u) the number %i.\n", j, fork_pids[j], randNumber);
			
			/*
			 * Signal the child to read the given number.
			 */
			ping(fork_pids[j], SIGINT);
		}
	}
	
	return 0;
}

/*
 * The handler for the SIGCHLD signal.
 */
void sigChildHandler() {
	pid_t wait_pid, exit_status;
	static int complete_count = 0;
	int l,i; // counters
	
	
	wait_pid = wait( &exit_status );
	exit_status <<= 16; exit_status >>= 24;	// exit code is only given 8-bit space, packed in 3rd byte
	
	for (l=0; l<numberOfChildren; l++) {
		if (fork_pids_ptr[l] == wait_pid) {
			/*
			 * BONUS: The child's PID and its sum is shown at the moment of its exit.
			 */
			printf("Parent here, Child #%u (PID %u) exited with a sum of %u. \n", l, wait_pid, exit_status );
			
			/*
			 * Keep track of child completion.
			 */
			completed_ptr[l] = true;
			complete_count++;
			
			free( outgoingPipes_ptr[l] ); // free the memory for the now unused pipe.
			
			/*
			 * BONUS: Exit only when all children have finished and exited.
			 */
			if (complete_count == numberOfChildren) {
				exit(0);
			}
			break;
		}
	}
}














