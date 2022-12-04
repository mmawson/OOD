#pragma once

#include <vector>

class Controller
{
  public:
    Controller();

    ~Controller::Controller();

  private:
    bool ConnectToStub(const size_t port);

    std::vector<int> mConnectedSockets;
};
