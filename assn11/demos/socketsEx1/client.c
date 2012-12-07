// IPC: Sockets
// SocketExample1 - get a webpage from www.csus.edu/admission
// client.c

#include <stdio.h>
#include <unistd.h>
#include <string.h>     // memcpy()
#include <netdb.h>      // struct sockaddr_in, ...
#include <sys/socket.h> // those socket calls

#define STR_LEN 512           // # of bytes, socket buffer size

#define PORT_NUM 1234           // 80 httpd, 110 pop mail
#define SERVER_IP "google.com\0" // www.csus.edu
// "72.30.38.140\0"           // www.yahoo.com
// "130.86.67.243\0"          // gaia.ecs.csus.edu
// "130.86.246.82\0"          // www.ecs.csus.edu

void main()
{
   int len_read, my_session;  // bytes read, connection session
   char str[ STR_LEN ];       // general str
   struct hostent *my_server; // host entry data structure
   struct sockaddr_in my_server_sock_addr; // socket addr (internet)

   my_session = socket( AF_INET, SOCK_STREAM, 0 ); // get descriptor
   my_server = gethostbyname( SERVER_IP );         // get host entry by IP

// prep my_server_sock_addr
   memcpy( &my_server_sock_addr.sin_addr.s_addr,
           my_server->h_addr, my_server->h_length );
   my_server_sock_addr.sin_family = AF_INET;
   my_server_sock_addr.sin_port = htons( PORT_NUM ); // Host to Network Service port #

// connect service described in my_server_sock_addr -> enable my_session
   if( connect( my_session, (struct sockaddr *)&my_server_sock_addr,
                sizeof(struct sockaddr) ) < 0 )
   {
      perror( "connect" );
      return;
   }

   sleep(1);   // give remote time to complete connect request

   write( my_session, "GET /\n" , 15 ); // request web page /index.html

   while( 1 ) // return when reading empty line
   {
      len_read = recv( my_session, str, STR_LEN, 0 ); // recv() doesn't block!
      if( len_read == 0 ) break; // EOF indication, server sends null str
      write( 1, str, len_read ); // show what's read from socket
   }
   close( my_session );
}

