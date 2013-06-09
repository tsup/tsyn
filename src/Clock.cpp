#include <tsyn/Clock.hpp>
#include <chrono>

tsyn::Clock::Clock()
  : m_start( InternalClockType::now() )
{
}

tsyn::Clock::Time
tsyn::Clock::now() const
{
  std::chrono::microseconds delta(
      std::chrono::duration_cast< std::chrono::microseconds >(
      InternalClockType::now() - m_start ) );
  return delta.count();
}

