#pragma once

#include <cstdint>

namespace MapReduce
{
  enum class ControllerMessageType : std::uint8_t
  {
    CREATE_MAP_INSTANCE = 0,
    CREATE_REDUCE_INSTANCE,
    TERMINATE_STUB
  };

  //Defines the message format for messages from the Controller to Stubs
  struct ControllerMessage
  {
    ControllerMessage(ControllerMessageType type) : type(type) {}
    //Only a single command for now --- can expand this class if we need to pass more info to stub
    ControllerMessageType type;
  };

  //Defines the message format for heartbeat messages from the Stub to the Controller
  struct StubHeartbeatMessage
  {
    std::uint8_t stubID;
  };


}//namespace MapReduce
