#pragma once

#include <cstdlib>
#include <string>
#include "Functions.h"

namespace MapReduce
{

class Stub
{
public:
  Stub(const std::size_t port);
  ~Stub();
  maker_t* FuncMap;

  static maker_t* accessMapLib();

private:
  bool ListenForConnection(std::size_t port);
  bool ListenForMessages();
  const int R = 2;

  void mapProcess();
//  const std::string pathToMapLib = "./lib/libmapNew.so";
//  std::string pathToReduceLib = "./lib/libreduceNew.so";
  int mListeningSocket;
  int mConnectedSocket;

  void reduceProcess();
};

}//namespace MapReduce
