#include <iostream>

/* Socket Programming Libaries */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
/* End Socket Programing Libraries */

namespace MapReduce
{

  Stub::Stub()
  {
    int obj_socket = 0, reader;
    struct sockaddr_in serv_addr;
    char buffer[1024]; // = {0};
    if (( obj_socket = socket (AF_INET, SOCK_STREAM, 0 )) < 0)
    {
      std::cout << "Socket creation error !" << std::endl;
      return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    // Converting IPv4 and IPv6 addresses from text to binary form
    if(inet_pton ( AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf ( "\nInvalid address ! This IP Address is not supported !\n" );
        return -1;
    }
    if ( connect( obj_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr )) < 0)
    {
        printf ( "Connection Failed : Can't establish a connection over this socket !" );
        return -1;
    }
    while (1) {
        reader = read ( obj_socket, buffer, 1024 );
        if (reader != 0) {
            if (!strcmp(buffer,"Map")) {
                cout << "Run a Map instance\n";
            }
            else if (!strcmp(buffer,"Reduce")) {
                cout << "Run a reduce instance\n";
            }
        }
    }
  }



}//namespace MapReduce
