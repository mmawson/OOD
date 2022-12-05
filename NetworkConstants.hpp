#pragma once

#include <string>

namespace MapReduce
{

  constexpr size_t STUB_LISTEN_PORT1 = 8080;
  constexpr size_t STUB_LISTEN_PORT2 = 8081;
  constexpr size_t STUB_LISTEN_PORT3 = 8082;

  //all on the same machine, so use loopback IP
  const std::string STUB_IP_ADDRESS = "127.0.0.1";

};
