#pragma once

#include <tsyn/Clock.hpp>

#include "Types.hpp"

namespace tsyn
{
  class Connection
  {
    public:
      Connection( LowLevelConnectionRef lowLevelConn, ReceiveQueue & recvQueue );

      void send( const tsyn::Data & payload, tsyn::Clock::Time timestamp );
      void receive( const tsyn::Data & receivedData );
    private:
      LowLevelConnectionRef m_lowLevelConn;
      ReceiveQueue & m_receiveQueue;
  };
}
