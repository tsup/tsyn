#pragma once

#include "Types.hpp"

namespace tsyn
{

  class LowLevelConnection
  {
    public:
      virtual void send( const Data& data ) = 0;
      virtual ~LowLevelConnection() {}
  };

}

