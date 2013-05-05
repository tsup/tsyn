#pragma once

#include "Types.hpp"

namespace tsyn
{
  class Connection;

  class LowLevelConnection
  {
    public:
      virtual void start( Connection& ownerConnection ) = 0;
      virtual void send( Data&& data ) = 0;
      virtual ~LowLevelConnection() {}
  };

}

