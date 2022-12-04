/* Socket Programming Libaries */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
/* End Socket Programing Libraries */

#include "NetworkConstants.hpp"
#include "Messages.hpp"

namespace MapReduce
{

  Controller::Controller()
  {
    ConnectToStub(STUB_LISTEN_PORT1);
    ConnectToStub(STUB_LISTEN_PORT2);
    ConnectToStub(STUB_LISTEN_PORT3);

    ControllerMessage createMapMsg(ControllerMessageType::CREATE_MAP_INSTANCE);
    ControllerMessage createReduceMsg(ControllerMessageType::CREATE_REDUCE_INSTANCE);

    send(mConnectedSockets[0], createMapMsg.type, strlen(createMapMsg.type), 0);
    send(mConnectedSockets[1], createMapMsg.type, strlen(createMapMsg.type), 0);
    send(mConnectedSockets[2], createMapMsg.type, strlen(createReduceMsg.type), 0);
  }

  ~Controller::Controller()
  {
    for (int socket_fd : mConnectedSockets)
    {
      socket_fd.close();
    }
  }

  bool ConnectToStub(const size_t port)
  {
    int sock = 0, valread, client_fd;

    struct sockaddr_in serv_addr;
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
      std::cout << "Socket creation error" << std::endl;
      return -1;
    }

    serv_addr.sin_familt = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, STUB_IP_ADDRESS.c_str(), &serv_addr.sin_addr) <= 0)
    {
      std::cout << "Invalid address" << std::endl;
      return -1;
    }

    if ((client_fd = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) 
    {
      std::cout << "Connection Failed" << std::endl;
      return -1;
    }

    mConnectedSockets.push_back(client_fd);
  }

}//namespace MapReduce
