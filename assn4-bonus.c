



#include <stdio.h> // input/output functions.
#include <string.h> // string functions
#include <stdint.h> // provides constant width integers (cross-platform)
#include <stdlib.h> // sprintf(),
#include <time.h> // to get the time.
#include <signal.h> // to use SIGKILL



int main(int argc, char *args[]) {
	
	if (argc < 3) {
		printf("Usage: %s arg1 arg2\n", args[0]);
		exit(1);
	}

	unsigned int numberOfChildren = atoi(args[1]);
	pid_t fork_pids[numberOfChildren];
	unsigned int minFinalSum = atoi(args[2]);
	unsigned int sum=0;
	
	pid_t wait_pid, exit_status;

	/*Let's start our randomness based on the current time. */
	srand( (unsigned int)time(NULL) );
	
	/*
	 * Create the three children processes.
	 */
	unsigned int i=0;
	int randNum = 0;
	for (i=0; i<numberOfChildren; i++) {
		fork_pids[i] = fork();
		
		/*
		 * Let's try to be a bit more random.
		 */
		int j=0; for (j=0; j<rand()%100+50; j++) { srand( rand() ); }
		// I know the above ^^^ is not best for performance... But how do I make numbers *more* random?
		
		if (fork_pids[i] == -1) {
			perror("fork"); // out of memory?
			exit(1);
		}
		else if (fork_pids[i] == 0) { // 0 means child process.
			while( sum < minFinalSum ) {
				sleep(1); //sleep one second
				randNum = rand() % 10; //get random number between 0 and 9
				sum = sum + randNum; //increment sum by random number.
				printf("Child %u (PID: %u) -- Current sum: %u\n", i, getpid(), sum); //display child proc id and current sum
			}
			exit(sum);
		}
	}
	
	/*
	 * Wait for the winner.
	 */
		wait_pid = wait( &exit_status );
		for (i=0; i<numberOfChildren; i++) {
			if (
			fork_pids[i] 
			!= wait_pid) {
				kill(fork_pids[i], SIGKILL);
			}
		}
		// exit code is only given 8-bit space, packed in 3rd byte
		exit_status <<= 16;
		exit_status >>= 24;
		printf("Parent here (PID %i) -- Child %i was the winner (sum == %i).\n", getpid(), wait_pid, exit_status );
	
}














