# Makefile
# AlphaDerby - Shared Memory (TwoTermsPick)

CMD_DELETE = rm -f
CC = gcc
MODEL = -g
#CFLAGS = $(MODEL) -O1 -Wall
CFLAGS = $(MODEL)
LFLAGS = $(MODEL)

LIBS = -lrt -pthread

# ---------------------------------------------------

all:	server alphabet client keypoll
	$(CMD_DELETE) *.o
	chmod 755 server alphabet client keypoll

clean:	$(CMD_DELETE) *.o server alphabet client keypoll *~ #*#

# ---------------------------------------------------

server:	server.o common.o
	$(CC) $(LFLAGS) -o $@ server.o common.o $(LIBS)

alphabet: alphabet.o common.o
	$(CC) $(LFLAGS) -o $@ alphabet.o common.o $(LIBS)

client:	client.o common.o
	$(CC) $(LFLAGS) -o $@ client.o common.o $(LIBS)

keypoll: keypoll.o
	$(CC) $(LFLAGS) -o $@ keypoll.o $(LIBS)

