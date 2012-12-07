// IPC: Sockets
// SocketExample2 - talk between 2 processes on remote internet locations
// client.c
// client can be anywhere on the internet to connect to server
// remote users can type/send strings between, back and forth

#include <stdio.h>
#include <unistd.h>
#include <string.h>     // memcpy()
#include <netdb.h>      // struct sockaddr_in, ...
#include <sys/socket.h> // those socket calls

#define STR_LEN 512       // # of bytes, socket buffer size
#define PORT_NUM 7777     // port # over 1023 are user (not root) ports

#define SERVER_IP "127.0.0.1\0"      // gaia.ecs.csus.edu

void main()
{
   struct sockaddr_in my_server_sock_addr;
   struct hostent *my_server;
   int len_read, my_session;
   char str[STR_LEN];

   my_session = socket( AF_INET, SOCK_STREAM, 0 ); // get descriptor
   my_server = gethostbyname( SERVER_IP );           // get entry by IP

// prep my_sock_addr
   memcpy( &my_server_sock_addr.sin_addr.s_addr,
           my_server->h_addr, my_server->h_length );
   my_server_sock_addr.sin_family = AF_INET;
   my_server_sock_addr.sin_port = htons( PORT_NUM );

// connect service described in my_server_sock_addr -> enable my_session
   if( connect( my_session, (struct sockaddr *)&my_server_sock_addr,
                sizeof(struct sockaddr) ) < 0 )
   {
      perror( "connect" );
      return;
   }

   sleep(1);   // give remote time to complete connect request

   while(1)    // return when reading empty line
   {
// recv from remote doesn't block (if call before server send(), 0 byte read)
      len_read = recv( my_session, str, STR_LEN, 0 );
      str[len_read] = 0;     // add null to know where string ends
      printf( "   Recv: %s\n", str );

      if( strcmp( str, "bye\0" ) == 0 ) break; // server confirms bye

// send to remote
      write( 1, "   Send: ", 14 );              // prompt user to type
      len_read = read( 0, str, STR_LEN );       // read from KB
      send( my_session, str, len_read - 1, 0 ); // send 1 less (skip \n)
   }
   close( my_session ); // shutting down
}

