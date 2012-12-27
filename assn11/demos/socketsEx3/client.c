// client.c
// Example 3
// recv() is non-blocking!

#include "common.h"

void main()
{
   int num, my_session, len_recv;

   struct sockaddr_in my_server_sock_addr;
   struct hostent *my_server;

   my_session = socket( AF_INET, SOCK_STREAM, 0 ); // get descriptor
   my_server = gethostbyname( SERVER_IP );         // get entry by IP

// prep my_sock_addr
   memcpy( &my_server_sock_addr.sin_addr.s_addr,
           my_server->h_addr, my_server->h_length );
   my_server_sock_addr.sin_family = AF_INET;
   my_server_sock_addr.sin_port = htons( PORT_NUM );

// connect service described in my_server_sock_addr -> enable my_session
   if( connect( my_session, (struct sockaddr *)&my_server_sock_addr,
                sizeof(struct sockaddr) ) < 0 )
   { perror( "connect" ); return; }

// starts receiving tests

   len_recv = 1;
   while( len_recv != 0 ) // indicates socket connection closed
   {
      len_recv = recv( my_session, &num, sizeof(int), 0 );
      printf("Using recv(), num received is %d, len_recv is %d\n",
             num, len_recv);
   }

   close( my_session );

} // end main
