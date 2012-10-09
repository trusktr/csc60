





#include <stdio.h>
#include <stdlib.h>



int main () {

	printf("%i", sizeof(int));

	printf("hee\n");
	fork();
		sleep(1);
	printf("hi\n");
	fork();
		sleep(1);
	printf("ho\n");
		sleep(1);

	int i, pid;
	for (i=0; i<3; i++) {
		fork();
		printf("%i\n", i);
		sleep(1);
	}
}














