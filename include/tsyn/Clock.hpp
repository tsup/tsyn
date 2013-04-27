#pragma once

#include <cstdint>
#include <chrono>

namespace tsyn
{

  class Clock
  {
    public:
      typedef std::uint64_t Time;
      Clock();
      Time now() const;

    private:
      typedef std::chrono::steady_clock InternalClockType;
      std::chrono::time_point< InternalClockType > m_start;
  };

}

