#include "Controller.hpp"

/* Socket Programming Libaries */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
/* End Socket Programing Libraries */

#include "NetworkConstants.hpp"
#include "Messages.hpp"

#include <cstring>
#include <iostream>

namespace MapReduce
{

  Controller::Controller()
  {
    ConnectToStub(STUB_LISTEN_PORT1);
    ConnectToStub(STUB_LISTEN_PORT2);
    ConnectToStub(STUB_LISTEN_PORT3);

    ControllerMessage createMapMsg(ControllerMessageType::CREATE_MAP_INSTANCE);
    ControllerMessage createReduceMsg(ControllerMessageType::CREATE_REDUCE_INSTANCE);

    send(mConnectedSockets[0], createMapMsg.Serialize(), sizeof(createMapMsg), 0);
    send(mConnectedSockets[0], createReduceMsg.Serialize(), sizeof(createReduceMsg), 0);
    send(mConnectedSockets[1], createMapMsg.Serialize(), sizeof(createMapMsg), 0);
    send(mConnectedSockets[2], createReduceMsg.Serialize(), sizeof(createReduceMsg), 0);

    while (true)
    {
      //Just stay alive until shut down
    }
  }

  Controller::~Controller()
  {
    for (int socket_fd : mConnectedSockets)
    {
      close(socket_fd);
    }
  }

  bool Controller::ConnectToStub(const std::size_t port)
  {
    int sock = 0, valread, client_fd;

    struct sockaddr_in serv_addr;
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
      std::cout << "Socket creation error" << std::endl;
      return false;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, STUB_IP_ADDRESS.c_str(), &serv_addr.sin_addr) <= 0)
    {
      std::cout << "Invalid address" << std::endl;
      return false;
    }

    if ((client_fd = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) 
    {
      std::cout << "Connection Failed" << std::endl;
      return false;
    }

    mConnectedSockets.push_back(client_fd);
    std::cout << "Connected on port " << port << std::endl;

    return true;
  }

}//namespace MapReduce
