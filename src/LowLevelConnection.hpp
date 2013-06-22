#pragma once

#include "Types.hpp"

namespace tsyn
{
  class Connection;
  class Endpoint;

  class LowLevelConnection
  {
    public:
      virtual const Endpoint& remoteEndpoint() const = 0;
      virtual void start( Connection& ownerConnection ) = 0;
      virtual void send( Data&& data ) = 0;
      virtual ~LowLevelConnection() {}
  };

}

