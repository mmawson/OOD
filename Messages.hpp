#pragma once

#include <cstdint>
#include <cstring>

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
    ControllerMessage() {}
    ControllerMessage(ControllerMessageType type) : type(type) {}
    //Only a single command for now --- can expand this class if we need to pass more info to stub
    ControllerMessageType type;

    //Serialize this message object for sending over a socket
    //TODO: This will leak memory -- need to modify to make sure to either reuse or clean up the messages we are serializing
    char* Serialize()
    {
      //track the position in the buffer as we write fields
      size_t pos = 0;

      //Allocate space for the message
      char* buffer = new char[sizeof(type)];

      //Copy over variable
      std::memcpy(buffer, &type, sizeof(type));

      return buffer;
    }

    void Deserialize(char* buffer)
    {
      //Copy the buffer into this object's member variable
      std::memcpy(&type, buffer, sizeof(type));
    }
  };

  //Defines the message format for heartbeat messages from the Stub to the Controller
  struct StubHeartbeatMessage
  {
    std::uint8_t stubID;
  };


}//namespace MapReduce
