#pragma once

#include <tsyn/Clock.hpp>

#include "Types.hpp"

namespace tsyn
{
  class Connection
  {
    public:
      Connection( SenderRef sender );
    private:
      SenderRef m_sender;
  };
}
