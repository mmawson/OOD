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
  

  void mapProcess(int);
//  const std::string pathToMapLib = "./lib/libmapNew.so";
//  std::string pathToReduceLib = "./lib/libreduceNew.so";
  int mListeningSocket;
  int mConnectedSocket;
  //const int R = 3;
  void reduceProcess();
};

}//namespace MapReduce
