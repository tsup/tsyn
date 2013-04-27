#include <igloo/igloo_alt.h>
using namespace igloo;

#include <tsyn/Clock.hpp>
#include <type_traits>
#include <thread>
#include <chrono>

Describe( tsyn_clock )
{
  template < typename Time >
  struct TimeTypeCheck
  {
    TimeTypeCheck() {};
    static_assert( std::is_integral< Time >::value, "Type of time should be an integral type." );
    static_assert( ! std::is_signed< Time >::value, "Type of time should be unsigned." );
    static_assert( std::numeric_limits< Time >::digits == 64, "Type of time should have size of 64 bits." );
  };

  It ( should_define_an_unsigned_64_bit_time_type )
  {
    typedef tsyn::Clock::Time TimeType;
    TimeTypeCheck< TimeType > check;
  }

  It ( should_return_Time_Type_from_now_function )
  {
    tsyn::Clock clk;
    typedef decltype( clk.now() ) TimeType;
    TimeTypeCheck< TimeType > check;
  }

  Describe ( returned_timestamp )
  {

    It ( should_increase_with_at_least_microsec_precision )
    {
      tsyn::Clock clk;
      tsyn::Clock::Time t0( clk.now() );
      std::this_thread::sleep_for( std::chrono::microseconds( 1 ) );
      tsyn::Clock::Time t1( clk.now() );
      AssertThat( t1, Is().GreaterThan( t0 ) );
    }

  };

};

