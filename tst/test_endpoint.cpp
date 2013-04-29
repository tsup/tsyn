#include <igloo/igloo_alt.h>
using namespace igloo;

#include <system_error>
#include <Endpoint.hpp>

Describe( AnEndpoint )
{
  It(can_parse_protocol_host_and_port)
  {
    tsyn::Endpoint endpoint( "tcp://80.23.66.17:2013" );

    AssertThat( endpoint.protocol(), Equals( tsyn::Proto::TCP ) );
    AssertThat( endpoint.ipAddress(), Equals( "80.23.66.17" ) );
    AssertThat( endpoint.port(), Equals( 2013 ) );
  }

  It(can_parse_udp_as_well)
  {
    tsyn::Endpoint endpoint( "udp://34.56.88.22:9888" );

    AssertThat( endpoint.protocol(), Equals( tsyn::Proto::UDP ) );
    AssertThat( endpoint.ipAddress(), Equals( "34.56.88.22" ) );
    AssertThat( endpoint.port(), Equals( 9888 ) );
  }

  It(should_throw_exception_if_protocol_is_unknown)
  {
    AssertThrows( std::system_error,
      tsyn::Endpoint endpoint( "xuuu://1.1.1.1:8080" )
    );
  }
};

