#pragma once

#include <string>
#include <boost/asio.hpp>

namespace tsyn
{
  enum class Proto
  {
    UNDEF = 0,
    TCP,
    UDP
  };
  class Endpoint
  {
    public:
      typedef uint16_t Port;
      Endpoint( const std::string & endpointStr );

      Proto protocol() const;
      const std::string& ipAddress() const;
      Port port() const;

      const std::string& asStr() const;
    private:
      void parse( const std::string & );

      Proto m_protocol;
      std::string m_ipAddress;
      Port m_port;

      const std::string m_endpointStr;
  };


  template < class BoostType >
  inline const char * protocolFromBoostEndpoint()
  {
    assert( 0 );
  }


  template<>
  inline const char * protocolFromBoostEndpoint< boost::asio::ip::udp::endpoint >()
  {
    return "udp";
  }


  template<>
  inline const char * protocolFromBoostEndpoint< boost::asio::ip::tcp::endpoint >()
  {
    return "tcp";
  }


  template< class BoostEndpoint >
  inline const Endpoint endpointFromBoost( const BoostEndpoint& boostEndpoint )
  {
    std::stringstream remoteStream;
    remoteStream
      << protocolFromBoostEndpoint<BoostEndpoint>() << "://"
      << boostEndpoint.address().to_string() << ":"
      << boostEndpoint.port();
    return Endpoint( remoteStream.str() );
  }

}
