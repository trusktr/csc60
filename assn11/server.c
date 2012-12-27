// server.c
// AlphaDerby - Shared Memory (TwoTermsPick)

#include "common.h"




typedef struct {
	int* pSessions;
	int* pSessionCount;
	int* pMy_sock_desc;
	void * pClient_sock_addr;
	int * pSock_size;
} threadStuff;

void* acceptConnections(void *arg) {
	threadStuff stuff = *(threadStuff*)arg;
	while (1) {
		stuff.pSessions[(*stuff.pSessionCount)++] = accept(*stuff.pMy_sock_desc, (struct sockaddr *)stuff.pClient_sock_addr, stuff.pSock_size );
	}
}




void main()
{
	sem_t *sem_video;         // video/stdout mutex
	char *p, str[30];         // for general purposes
	int shmids[26];           // 26 shared info_t areas
	info_t *infos[26];        // 26 ptrs to info of each alphabet
	int finished, places[26];    // count how many done, their places
	int *cols4client, shmid4client; // for player to view 26 col #'s
	char* pick[MAX_CLIENTS];               // cols4client[26+(2n)] represents this
	int* points[MAX_CLIENTS];              // cols4client[27+(2n)] represents this
	int i, j; //counters















   /*
    * SET UP SOCKET STUFF
    */
   int true, sock_size, my_sock_desc, sessions[MAX_CLIENTS], sessionCount = 0;        // socket use
   struct sockaddr_in my_sock_addr, client_sock_addr; // socket use
   int num = 0;            // counter

// open/request socket like asking for descriptor from OS
   if ( -1 == ( my_sock_desc = socket( AF_INET, SOCK_STREAM, 0 ) ) )
   { perror("socket"); return; }

// set socket option: addr, port, etc.
   if( -1 == setsockopt( my_sock_desc, SOL_SOCKET, SO_REUSEADDR, &true, sizeof( int ) ) )
   { perror("setsockopt"); return; }

   my_sock_addr.sin_family = AF_INET;          // Internet AF type socket
   my_sock_addr.sin_port = htons( PORT_NUM );  // Host TO Network Service (htons)
   my_sock_addr.sin_addr.s_addr = INADDR_ANY;  // Internet address as server addr
   bzero( &my_sock_addr.sin_zero, 8 );  // 8 bytes or sizeof(my_sock_addr.sin_zero)

// bind server socket to Internet addr
   if ( -1 == bind( my_sock_desc, (struct sockaddr *)&my_sock_addr, sizeof(struct sockaddr) ) )
   { perror("bind"); return; }



















   sprintf(str, "%d", getpid());
   sem_video = sem_open( str, O_CREAT, 0600, 1 ); // mutex type
   if( sem_video == SEM_FAILED ) { perror("sem_open"); return; }

   for( i=0; i<26; i++ ) // each alphabet needs info_t
   {
// request/allocate shared memory, size is one info_t
      if( ( shmids[i] = shmget( getpid()+'A'+i, sizeof(info_t), IPC_CREAT | 0600 ) ) < 0 )
      { perror( "shmget #1" ); return; }

// attach shared memory segment to local data
      if( ( p = shmat( shmids[i], 0, 0 ) ) == (char *) -1 )
      { perror( "shmat #1" ); return; }

      infos[i] = (info_t *)p;
      infos[i]->symbol = 'A'+i; // prep info_t for alphabet
      infos[i]->row = i+1;
   }

// for player client, 26 cols + MAX_CLIENTS picks + MAX_CLIENTS points
   if( ( shmid4client = shmget( getpid()+1, sizeof(int[26+(MAX_CLIENTS*2)]), IPC_CREAT | 0600 ) ) < 0 )
   { perror( "shmget #2" ); return; }

// attach shared memory segment to local data
   if( ( p = shmat( shmid4client, 0, 0 ) ) == (char *) -1 )
   { perror( "shmat #2" ); return; }

   cols4client = (int *)p; // an array of int, 26 col #'s // size: 28

   // QUESTION: How do we know that the items in cols4client[] are all zero?
   // I'm wondering because in ./client.c we assume that items cols[] are all zero in order to compare to olds[].

// let client pick winner
	for (i=0; i<MAX_CLIENTS; i++) {
		pick[i] = (char *)&cols4client[26+(2*i)];	// last 2 allocated int spaces
		points[i] = &cols4client[27+(2*i)];			// are used for bidding/scoring
		*pick[i] = *points[i] = 0;					// not picked and no points won
	}







	/*
	 * LISTEN FOR CLIENTS TO SEND THEM THEIR OWN shmid4client.
	 */
   if ( -1 == listen( my_sock_desc, 5 ) )
   { perror("listen"); return; }

// listen blocks (awaiting/accepting client to open connection sessions)
   printf( "   Listening on port %d...\n", PORT_NUM );
   sock_size = sizeof( struct sockaddr_in ); // Internet socket type

	/*
	 * ACCEPT CONNECTIONS FOR CONNECTION_TIME SECONDS:
	 */
	threadStuff forTheThread;
	forTheThread.pSessions = sessions;
	forTheThread.pSessionCount = &sessionCount;
	forTheThread.pMy_sock_desc = &my_sock_desc;
	forTheThread.pClient_sock_addr = &client_sock_addr;
	forTheThread.pSock_size = &sock_size;
	pthread_t pthConnectionListener;
	pthread_create(&pthConnectionListener, NULL, acceptConnections, (void*)&forTheThread);
	printf( "\nFrom other shells, run clients and pick a letter: './client' "/*, PORT_NUM*/ ); // No arguments, PORT_NUM is constant in common.h
	fflush(stdout);
	unsigned int currentTime = (unsigned int)time(NULL);
	while ( (unsigned int)time(NULL) - currentTime < CONNECTION_TIME ) {
		printf( "\n - Waiting for clients to connect for %u more seconds... ", CONNECTION_TIME - ( (unsigned int)time(NULL) - currentTime ) );
		sleep(1);
		fflush(stdout);
	}
	pthread_cancel(pthConnectionListener);

// get info of client IP and Port # (ntoa - get IP, ntoh - get port #)
   printf( "   Got client from IP %s, Port %d...\n", inet_ntoa( client_sock_addr.sin_addr ), ntohs( client_sock_addr.sin_port ) );

   /*
    * SEND shmid4client TO CLIENTS
    */
	for( num = 0; num < sessionCount; num++ )
	{
		printf( "Sending client id to client #%i: %i\n", num, num );
		send( sessions[num], &num, sizeof(int), 0 );
		printf( "Sending shmid4client to client #%i: %d\n", num, shmid4client );
		send( sessions[num], &shmid4client, sizeof(int), 0 );
		sleep(1);
	}

	printf("DONE SENDING shmid4client TO CLIENTS........\n");







	printf( "\n" );
	for (i=0; i<sessionCount; i++) {
		while( (int)*pick[i] == 0 ) {
			usleep(500000); // .5 sec check-wait loop
			printf("Waiting for client %i to pick...\n", i);
		}

		// QUESTION: How do we know that, from the above line to the below line, the client isn't done setting *pick in memory?
		printf( "\n   >>>>> Client #%i picked %c <<<<< ", i, *pick[i] );
		fflush(stdout);
	}
	sleep(2);

// race starts from here
   InitScr( sem_video );

   for( i=0; i<26; i++ )
   {
      switch( fork() )
      {
         case -1: perror("fork"); return;
         case 0:
            sprintf( str, "%d", shmids[i] );
            execl( "./alphabet", "./alphabet", str, 0 ); return;
      }
   }
// each places[?] will be set when proc reaches goal
   for( i=0; i<26; i++ ) places[i] = 0; // not set
   finished = 0;
   while( finished < 26 ) // loop until all accounted for
   {
      for( i=0; i<26; i++ ) // check all proc status
      {
// this is for player client to view
         cols4client[i] = infos[i]->col;

// when proc reaches goal and 1st time we see this (places[?]==0)
         if( places[i] == 0  && infos[i]->col == MAX_COL )
         {
            finished++;           // this is also overall placement
            places[i] = finished; // marked its placement

            // QUESTION: What if two letters finish almost at the same time, will server mistake their positions?

// 1st 5 will get *** shown in its placement
            if( finished <= 5 ) sprintf( str, "%c[%d;%dH <-- %d ***",
                     27, i+1, MAX_COL+1, finished );
            else sprintf( str, "%c[%d;%dH <-- %d",
                     27, i+1, MAX_COL+1, finished );

            sem_wait(sem_video);
            write( 1, str, strlen(str) );   // make it happen
            sem_post(sem_video);

// check if client's pick wins
            for (j=0; j<MAX_CLIENTS; j++) {
	            if ( 'A'+i == *pick[j] ) *points[j] = 5 - finished + 1;
	        }

         } // end if
      } // end for( symbol=...
   } // end while( place < 26 )

   sleep(1); // don't flashing right away
   Flash();  // will also move cursor below things (to end cleanly)

	for (j=0; j<MAX_CLIENTS; j++) {
		if( *points[j] > 0 ) printf( "Client #%i: You've won %d points!\n", j, *points[j] );
		else printf( "Client #%i: Better luck next time!\n ", j);
	}

// close semaphore sem_video (use key str that opened it) and all
// shared mem requested via shmids[] and shmid4client
   sprintf( str, "%d", getpid() );   // use pid as key
   sem_unlink( str );                // close sem_video

   for( i=0; i<26; i++ ) shmctl( shmids[i], IPC_RMID, 0 );

   shmctl( shmid4client, IPC_RMID, 0 );

   sleep(1);

   for (i=0; i<sessionCount; i++) {
	   close( sessions[i] ); // close connection sessions with remote client
   }

} // end main

