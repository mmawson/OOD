#pragma once

#include <cstdlib>

namespace MapReduce
{

class Stub
{
public:
  Stub(const std::size_t port);
  ~Stub();

private:
  bool ListenForConnection(std::size_t port);
  bool ListenForMessages();

  int mListeningSocket;
  int mConnectedSocket;
};

}//namespace MapReduce
