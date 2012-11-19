
	


all: parent child
	
parent: parent.o common.o common.h
	gcc -o $@ parent.o common.o -pthread -lrt


child: child.o common.o common.h
	gcc -o $@ child.o common.o -pthread -lrt
	
	
clean:
	rm *.o parent child #*~ #* #
