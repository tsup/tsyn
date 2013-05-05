#pragma once

#include <Types.hpp>

#include "Message.hpp"
#include <tsyn/Clock.hpp>

inline std::string extractBytes( uint64_t value, size_t numberOfBytes )
{
  std::string extracted;
  for ( int i = numberOfBytes-1; i >= 0; --i )
  {
    extracted += ( value >> i * 8 ) & 0xff;
  }
  return extracted;
}

inline tsyn::Data generateUserMessage( const tsyn::Data & payload, tsyn::Clock::Time ts )
{
  uint32_t msgLength = tsyn::Message::HEADER_LENGTH + payload.length();
  tsyn::Data message = extractBytes( msgLength, 4 );
  message += extractBytes( ts, 8 );
  message += tsyn::MessageType::USER;
  message += payload;
  return message;
}

