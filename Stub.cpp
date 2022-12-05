#include "Stub.hpp"

#include <iostream>
#include "Messages.hpp"

/* Socket Programming Libaries */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
/* End Socket Programing Libraries */

namespace MapReduce
{

  Stub::Stub(const size_t port)
  {
    if (!ListenForConnection(port)) { std::cout << "Error when listening for connection" << std::endl; }

    //Keep listening until the controller sends a termination message
    bool terminate = false;
    while (terminate == false)
    {
      terminate = ListenForMessages();
    }
  }

  Stub::~Stub()
  {
  }

  bool Stub::ListenForConnection(const size_t port)
  {
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
    //Create socket file descriptor
    if ((mListeningSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
      perror("socket failed");
      return false;
    }

    //Forcefully attahing socket to the port
    if (setsockopt(mListeningSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
      perror("setsockopt"); 
      return false;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(mListeningSocket, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
      perror("bind failed");
      return false;
    }
    if (listen(mListeningSocket, 3) < 0) 
    {
      perror("listen");
      return false;
    }

    if ((mConnectedSocket = accept(mListeningSocket, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0)
    {
      perror("accept");
      return false;
    }

    return true;
  }

  bool Stub::ListenForMessages()
  {
    char buffer[1024] = { 0 };
    int valread = read(mConnectedSocket, buffer, 1024); 

    if (valread == 0)
    {
      return false;
    }

    //TODO: Take the appropriate actions after receiving these messages

    ControllerMessage message;
    message.Deserialize(buffer);

    if (message.type == ControllerMessageType::CREATE_MAP_INSTANCE)
    {
      std::cout << "Got a message to create a map instance" << std::endl;
      return false;
    }
    else if (message.type == ControllerMessageType::CREATE_REDUCE_INSTANCE)
    {
      std::cout << "Got a message to create a reduce instance" << std::endl;
      return false;
    }
    else if (message.type == ControllerMessageType::TERMINATE_STUB)
    {
      std::cout << "Got a message to terminate this stub!" << std::endl;
      return true;
    }

    return false;
  }



}//namespace MapReduce
