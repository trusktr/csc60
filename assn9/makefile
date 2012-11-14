# Makefile for AlphaDerby

CMD_DELETE = rm -f
CC = gcc
MODEL = -g
#CFLAGS = $(MODEL) -O1 -Wall
CFLAGS = $(MODEL)
LFLAGS = $(MODEL)

LIBS = -lrt -pthread

# ---------------------------------------------------

all:	parent child
	$(CMD_DELETE) *.o
	chmod 755 parent child

clean:	$(CMD_DELETE) *.o parent child *~ #*#

# ---------------------------------------------------

parent:	parent.o common.o
	$(CC) $(LFLAGS) -o $@ parent.o common.o $(LIBS)

child:	child.o common.o
	$(CC) $(LFLAGS) -o $@ child.o common.o $(LIBS)
