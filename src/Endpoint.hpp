#pragma once

#include <string>

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
    private:
      void parse( const std::string & );

      Proto m_protocol;
      std::string m_ipAddress;
      Port m_port;
  };
}
