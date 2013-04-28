#pragma once

#include <tsyn/Clock.hpp>

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
  class Connection
  {
    public:
      Connection( SenderRef sender, ReceiveQueue & recvQueue );

      void send( const tsyn::Data & payload, tsyn::Clock::Time timestamp );
      void receive( const tsyn::Data & receivedData );
    private:
      SenderRef m_sender;
      ReceiveQueue & m_receiveQueue;
  };
}
