

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 5





int main(void) {

	int a, b, c;
	int arr[SIZE];
	int *p;
	int* *q;
	int i; //counter
	
	
	printf("&a == %u, &b == %u, &c == %u \n\n", &a, &b, &c);
	
	
	for (i=0; i<SIZE; i++)
		printf("&arr[%i] == %u \n", i, &arr[i]);
	printf("\n");
	
	
	srand( (unsigned int)time(NULL) );
	p = &a; *p = rand();
	p = &b; *p = rand();
	p = &c; *p = rand();
	
	
	printf("a == %i, b == %i, c == %i \n", a, b, c);
	printf("\n");
	
	
	//using p for both assigning and printing:
	p = arr;
	for (i=0; i<SIZE; i++)
		printf( "arr[%i] == %i \n", i, *p++ = rand() );
	printf("\n");
	
	
	//verification:
	for (i=0; i<SIZE; i++)
		printf("arr[%i] == %i \n", i, arr[i]);
	printf("\n");
	
	
	q = &p;
	*q = *q - SIZE;
	for (i=0; i<SIZE; i++) {
		if (i < 3) {
			printf( "arr[%i] == %i \n", i, **q = (i==0 ? a : (i==1 ? b : c)) );
		}
		else {
			printf( "arr[%i] == %i \n", i, **q);
		}
		*q = *q + 1;
	}
	printf("\n");
	
	
	//verification:
	for (i=0; i<SIZE; i++)
		printf("arr[%i] == %i \n", i, arr[i]);
	printf("\n");

}

























