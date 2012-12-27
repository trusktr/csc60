# Makefile

CMD_DELETE = rm -f
CC = gcc
MODEL = -g
#CFLAGS = $(MODEL) -O1 -Wall
CFLAGS = $(MODEL)
LFLAGS = $(MODEL)

LIBS = -lrt

# ---------------------------------------------------

all:	server client
	$(CMD_DELETE) *.o
	chmod 755 server client

clean:	$(CMD_DELETE) *.o server client *~ #*#

# ---------------------------------------------------

server:	server.o
	$(CC) $(LFLAGS) -o $@ server.o $(LIBS)

client:	client.o
	$(CC) $(LFLAGS) -o $@ client.o $(LIBS)

