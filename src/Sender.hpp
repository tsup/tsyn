#pragma once

#include "Types.hpp"

namespace tsyn
{

  class Sender
  {
    public:
      virtual void send( const Data& data ) = 0;
      virtual ~Sender() {}
  };

}

