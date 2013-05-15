#include <igloo/igloo.h>
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

  Context(malformed_IP_address)
  {
    Spec(IP_has_element_greater_than_255)
    {
      AssertThrows( std::system_error,
        tsyn::Endpoint endpoint( "tcp://89.1.455.2:9898" )
      );
    }

    Spec(IP_has_not_enough_element)
    {
      AssertThrows( std::system_error,
        tsyn::Endpoint endpoint( "tcp://1.219.2:9898" )
      );
    }

    Spec(IP_has_too_many_elements)
    {
      AssertThrows( std::system_error,
        tsyn::Endpoint endpoint( "tcp://1.2.4.219.2:9898" )
      );
    }
  };

  Context(invalid_port)
  {
    Spec(port_is_too_high)
    {
      AssertThrows( std::system_error,
        tsyn::Endpoint endpoint( "tcp://43.88.9.45:70000" )
      );
    }

    Spec(port_is_zero)
    {
      AssertThrows( std::system_error,
        tsyn::Endpoint endpoint( "tcp://43.88.9.45:0" )
      );
    }

    Spec(port_is_negative)
    {
      AssertThrows( std::system_error,
        tsyn::Endpoint endpoint( "tcp://43.88.9.45:-12" )
      );
    }

    Spec(port_is_text)
    {
      AssertThrows( std::system_error,
        tsyn::Endpoint endpoint( "tcp://43.88.9.45:http" )
      );
    }
  };

  Context(input_string_is_malformed)
  {
    Spec(random_bullshit)
    {
      AssertThrows( std::system_error,
        tsyn::Endpoint endpoint( "fortytwo dog ate my cap" )
      );
    }

    Spec(only_proto)
    {
      AssertThrows( std::system_error,
        tsyn::Endpoint endpoint( "tcp" )
      );

      AssertThrows( std::system_error,
        tsyn::Endpoint endpoint( "tcp:" )
      );
    }

    Spec(missing_IP)
    {
      AssertThrows( std::system_error,
        tsyn::Endpoint endpoint( "tcp://:1024" )
      );
    }

    Spec(only_separators)
    {
      AssertThrows( std::system_error,
        tsyn::Endpoint endpoint( "://...:" )
      );
    }
  };

  It(cannot_resolve_hostname)
  {
    AssertThrows( std::system_error,
      tsyn::Endpoint endpoint( "tcp://github.com:9898" )
    );
  }
};

