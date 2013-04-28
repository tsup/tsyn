#pragma once

#include "Types.hpp"

namespace tsyn
{
  constexpr uint32_t HEADER_LENGTH = 9;
  struct MessageType
  {
    enum MsgType
    {
      USER = 0x01
    };
  };
  struct Message
  {
    Clock::Time timestamp;
    uint8_t type;
    Data payload;
  };
  Data serialize( const Message& msg );
}

