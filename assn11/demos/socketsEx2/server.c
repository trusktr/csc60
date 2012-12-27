// IPC: Sockets
// SocketExample2 - talk between 2 processes on remote internet locations
// server listens and reply (typing strings to send), single client only
// server.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define STR_LEN 512
#define PORT_NUM 7777        // not 0-1023, a user app port
#define HELLO_MSG "Hello, enter 'bye' to stop connection.\0"

void main()
{
   char str[STR_LEN], r_str[STR_LEN], s_str[STR_LEN];
   int true, sock_size, len_read, pid,
       my_sock_desc, session;
   struct sockaddr_in my_sock_addr, client_sock_addr;    

   if ( -1 == ( my_sock_desc = socket( AF_INET, SOCK_STREAM, 0 ) ) )
   {
      perror("socket");
      return;
   }
   if( -1 == setsockopt( my_sock_desc, SOL_SOCKET, SO_REUSEADDR,
                         &true, sizeof( int ) ) )
   {
      perror("setsockopt");
      return;
   }
   my_sock_addr.sin_family = AF_INET;         
   my_sock_addr.sin_port = htons( PORT_NUM );     
   my_sock_addr.sin_addr.s_addr = INADDR_ANY; 
   bzero( &my_sock_addr.sin_zero, 8 );  // 8 bytes or sizeof(my_sock_addr.sin_zero)

   if ( -1 == bind(my_sock_desc, (struct sockaddr *)&my_sock_addr,
                   sizeof(struct sockaddr) ) )
   {
      perror("bind");
      return;
   }
   if ( -1 == listen( my_sock_desc, 5 ) )
   {
      perror("listen");
      return;
   }
		
   while(1) // an infinite service loop (we can change it if wanted)
   {  
// this blocks when listening (accepting but no clients)
      printf( "   Listening at port %d...\n", PORT_NUM );
      sock_size = sizeof( struct sockaddr_in );
      session = accept( my_sock_desc, (struct sockaddr *)&client_sock_addr,
                        &sock_size );

// if passing above accept call
      printf( "   Got client from IP %s, Port %d...\n",
         inet_ntoa( client_sock_addr.sin_addr ), ntohs( client_sock_addr.sin_port ) );

      sprintf( r_str, "   Recv (%s): ", inet_ntoa( client_sock_addr.sin_addr ) );
      sprintf( s_str, "   Send (%s): ", inet_ntoa( client_sock_addr.sin_addr ) );

// initiate conversation to avoid both waiting (calling recv)
      send( session, HELLO_MSG, sizeof(HELLO_MSG), 0 );  // break ice

      while(1)
      {
         len_read = recv( session, str, STR_LEN, 0 );
         str[ len_read ] = 0;      // add null to know where string ends
         printf( "%s%s\n", r_str, str );

         if( strcmp( str, "bye\0" ) == 0 ) // from remote, could be bye
         {
            send( session, "bye", 3, 0 );  // confirm bye
            close( session );              // close client session
            break;
         }
// if no \n to printf(), must fflush(stdout) or won't come out (displayed)
         write( 1, s_str, strlen(s_str) );       // prompt-to-enter str
         len_read = read( 0, str, STR_LEN );     // read KB (\n will be in)
         send( session, str, len_read - 1, 0 );  // skip last char (\n)

      } // connected session loop
   }
   close( my_sock_desc ); // shutting down
}

