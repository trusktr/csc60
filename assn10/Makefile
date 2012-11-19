# Makefile
# AlphaDerby - Shared Memory (Standard)

CMD_DELETE = rm -f
CC = gcc
MODEL = -g
#CFLAGS = $(MODEL) -O1 -Wall
CFLAGS = $(MODEL)
LFLAGS = $(MODEL)

LIBS = -lrt -pthread

# ---------------------------------------------------

all:	server alphabet
	$(CMD_DELETE) *.o
	chmod 755 server alphabet

clean:	$(CMD_DELETE) *.o server alphabet *~ #*#

# ---------------------------------------------------

server:	server.o common.o
	$(CC) $(LFLAGS) -o $@ server.o common.o $(LIBS)

alphabet: alphabet.o common.o
	$(CC) $(LFLAGS) -o $@ alphabet.o common.o $(LIBS)

