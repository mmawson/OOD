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

/*********** Must have already run "program.out stub 1 &" ***************/
/*********** the following command tellls stub 1 to make a Map thread and map the files ****/
    send(mConnectedSockets[0], createMapMsg.Serialize(), sizeof(createMapMsg), 0);    
//  Jedi Mind Trick here
    sleep(10);  // In between the time it takes to map the files we need a pause for the map to finish before we serialize
/*********** Must have already run "program.out stub 2 &" **************/
/*********** the following command tellls stub 1 to make a Map thread and map the files ****/
    send(mConnectedSockets[1], createReduceMsg.Serialize(), sizeof(createReduceMsg), 0);

/*    These commands can be uncommented to add another socket or more map and reduce threads */
//    send(mConnectedSockets[0], createReduceMsg.Serialize(), sizeof(createReduceMsg), 0);
//    send(mConnectedSockets[1], createReduceMsg.Serialize(), sizeof(createReduceMsg), 0);
//    send(mConnectedSockets[2], create.Serialize(), sizeof(createReduceMsg), 0);
//    send(mConnectedSockets[2], createReduceMsg.Serialize(), sizeof(createReduceMsg), 0);

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
    int sock = 0, valread;

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

    if ((connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) 
    {
      std::cout << "Connection Failed" << std::endl;
      return false;
    }

    mConnectedSockets.push_back(sock);
    std::cout << "Connected on port " << port << std::endl;

    return true;
  }

}//namespace MapReduce
