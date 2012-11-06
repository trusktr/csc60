
/*
 * Joe Pea -- Assignment 7 -- Weide Chang
 */

#include "common.h"

/*
 * Global variables.
 */
unsigned int numberOfChildren;
int complete_count = 0;
pid_t *fork_pids_ptr; // pointer to array of child PIDs.
bool *completed_ptr; // pointer to array of booleans representing each child's completion.
// MESSAGE QUEUE STUFF.
int msgQ; // holds the id of our message queue.
char msgQ_str[10]; // for the child to know the msgQ id.
msgQTransport messageBird;

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
	bool completed[numberOfChildren]; for (i=0; i<numberOfChildren; i++) completed[i] = false; // to keep track of children completion.
	/*global*/ completed_ptr = completed;
	
	/*
	 * If a message queue with the key of this pid already exists, remove it.
	 */
	if ( (msgQ = msgget( (key_t) getpid(), 0 )) != -1 ) {
		if( msgctl( msgQ, IPC_RMID, 0 ) == -1 ) {
			printf( "Parent: Message queue already exists but unable to remove it." );
			return;
		}
	}
		
	/*
	 * Get a new message queue.
	 */
	msgQ = msgget((key_t) getpid(), IPC_CREAT | 0666);
	sprintf(msgQ_str, "%u", msgQ); // so we can pass the message queue id to the child.
	printf("Parent here (PID %u), got message queue id %u.\n", getpid(), msgQ);
	
	/*
	 * Fork each child process.
	 */
	for (i=0; i<numberOfChildren; i++) { // fork the children
		fork_pids[i] = fork();
/*		fork_pids[i] = 2;*/
/*		printf(" -- test\n");*/
		
		if (fork_pids[i] == -1) { // -1 means error forking.
			printf("Parent: Error forking. \n"); // out of memory?
			kill(0, SIGKILL);
			exit(4);
		}
		else if (fork_pids[i] == 0) { // 0 means we're in the child process.
			sprintf(goalSum_str, "%u", goalSum);
			
			/*
			 * Use execl() to make each fork replace itself with an new process: "./child"
			 */
			execl("./child", msgQ_str, goalSum_str, 0 );
		}
	}
	
	/*
	 * Register sigChildHandler() as the handler for the SIGCHLD signal, when children exit.
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
			 * Skip this child if it has already exited.
			 */
			if (completed[j] == true) continue;
			
			usleep(600000);
			
			/*
			 * Seed random randomly to be more random.
			 */
			for (k=0; k<rand()%100+50; k++) srand( rand() );
			
			/*
			 * Send a random number to this child through the message queue.
			 */
			randNumber = rand() % 10;
			messageBird.type = fork_pids[j];
			messageBird.msg = randNumber;
			printf("\nParent here, sending child #%i (PID %u) the number %i.\n", j, fork_pids[j], randNumber);
			if( -1 == msgsnd( msgQ, (void *)&messageBird, MSG_SIZE, 0 ) ) {
				printf( "Parent: error sending message to child #%u. \n", j );
				kill(0, SIGKILL);
				exit(77);
			}
			
			/*
			 * Expect and receive a message back from this child, indicating it's current sum.
			 */
			if( -1 == msgrcv( msgQ, (void *)&messageBird, MSG_SIZE, getpid(), 0 ) ) {
				printf( "Parent: error receiving message from child #%u. \n", j );
				kill(0, SIGKILL);
				exit(77);
			}
			printf("Parent here, child #%i (PID %u) sent back the sum %i.\n", j, fork_pids[j], messageBird.msg);
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
			 * If child exits with an error, kill all children and quit.
			 */
			if (exit_status == 77) {
				kill(0, SIGKILL);
				exit(66);
			}
		
			/*
			 * The child's PID and its sum is shown at the moment of its exit.
			 */
			printf("Parent here, child #%u (PID %u) exited with a sum of %u. \n", l, wait_pid, exit_status );
			
			/*
			 * Keep track of child completion.
			 */
			completed_ptr[l] = true;
			complete_count++;
			
			/*
			 * Exit only when all children have finished and exited.
			 */
			if (complete_count == numberOfChildren) {
	
				/*
				 * Delete the message queue before exiting.
				 */
				if ( -1 == msgctl(msgQ, IPC_RMID, 0) ) {
					printf("Parent: failed to delete the message queue.\n");
					exit(66);
				}
				exit(0);
			}
			
			break;
		}
	}
}














