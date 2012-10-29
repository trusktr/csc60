

all: parent child
	rm *.o
	
parent: parent.o
	gcc -o $@ parent.o


child: child.o
	gcc -o $@ child.o
	
	
clean:
	rm *.o parent child #*~ #* #
