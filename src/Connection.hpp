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
      Connection( SenderRef sender );

      void send( const tsyn::Data & payload, tsyn::Clock::Time timestamp );
    private:
      SenderRef m_sender;
  };
}
