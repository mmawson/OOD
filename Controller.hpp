#pragma once

#include <vector>

namespace MapReduce
{

  class Controller
  {
    public:
      Controller();

      ~Controller();

    private:
      bool ConnectToStub(const std::size_t port);

      std::vector<int> mConnectedSockets;
  };

}//namespace MapReduce
