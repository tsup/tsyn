#pragma once

#include "Types.hpp"

namespace tsyn
{
  struct MessageType
  {
    enum MsgType
    {
      USER = 0x01
    };
  };
  struct Message
  {
    static constexpr uint32_t LENGTH_SIZE = 4;
    static constexpr uint32_t HEADER_LENGTH = 9;

    Clock::Time timestamp;
    uint8_t type;
    Data payload;
  };
  Data serialize( const Message& msg );
}

