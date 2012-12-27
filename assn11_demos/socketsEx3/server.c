// server.c
// Example 3
// recv() is non-blocking!

#include "common.h"

void main()
{
   int true, sock_size, my_sock_desc, session;        // socket use
   struct sockaddr_in my_sock_addr, client_sock_addr; // socket use
   int num;                // testing connection data seding/recving

// open/request socket like asking for descriptor from OS
   if ( -1 == ( my_sock_desc = socket( AF_INET, SOCK_STREAM, 0 ) ) )
   { perror("socket"); return; }

// set socket option: addr, port, etc.
   if( -1 == setsockopt( my_sock_desc, SOL_SOCKET, SO_REUSEADDR,
                         &true, sizeof( int ) ) )
   { perror("setsockopt"); return; }

   my_sock_addr.sin_family = AF_INET;          // Internet AF type socket
   my_sock_addr.sin_port = htons( PORT_NUM );  // Host TO Network Service (htons)
   my_sock_addr.sin_addr.s_addr = INADDR_ANY;  // Internet address as server addr
   bzero( &my_sock_addr.sin_zero, 8 );  // 8 bytes or sizeof(my_sock_addr.sin_zero)

// bind server socket to Internet addr
   if ( -1 == bind( my_sock_desc, (struct sockaddr *)&my_sock_addr,
                   sizeof(struct sockaddr) ) )
   { perror("bind"); return; }

// start listening port for clients
   if ( -1 == listen( my_sock_desc, 5 ) )
   { perror("listen"); return; }

// listen blocks (awaiting/accepting client to open connection session)
   printf( "   Listening at port %d...\n", PORT_NUM );
   sock_size = sizeof( struct sockaddr_in ); // Internet socket type
   session = accept( my_sock_desc, (struct sockaddr *)&client_sock_addr,
                     &sock_size );

// get info of client IP and Port # (ntoa - get IP, ntoh - get port #)
   printf( "   Got client from IP %s, Port %d...\n",
      inet_ntoa( client_sock_addr.sin_addr ), ntohs( client_sock_addr.sin_port ) );

// here we test sending 10 numbers to see if client can sync to it
   for( num = 0; num < 10; num++ )
   {
      printf( "Sending number %d\n", num );
      send( session, &num, sizeof(int), 0 );
      sleep(1);
   }

   sleep(1);
   close( session ); // close connection session with remote client
} // end main

