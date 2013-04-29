#include "Endpoint.hpp"

#include <system_error>

tsyn::Endpoint::Endpoint( const std::string & endpointStr )
  : m_protocol( Proto::UNDEF )
  , m_ipAddress( "" )
  , m_port( 0 )
{
  parse(endpointStr);
}

tsyn::Proto tsyn::Endpoint::protocol() const
{
  return m_protocol;
}

const std::string& tsyn::Endpoint::ipAddress() const
{
  return m_ipAddress;
}

tsyn::Endpoint::Port tsyn::Endpoint::port() const
{
  return m_port;
}

namespace
{
tsyn::Proto parseProto( const std::string & str )
{
  size_t semiColonIndex = str.find( ':' );
  if ( 0 == str.compare(0,semiColonIndex, "tcp" ) )
  {
    return tsyn::Proto::TCP;
  }
  if ( 0 == str.compare(0,semiColonIndex, "udp" ) )
  {
    return tsyn::Proto::UDP;
  }
  throw std::system_error();
}

tsyn::Endpoint::Port parsePort( const std::string & str )
{
  size_t lastSemiColon = str.find_last_of( ':' );
  return std::stoi( str.substr( lastSemiColon+1 ) );
}

std::string parseIpAddress( const std::string & str )
{
  size_t doubleSlashIndex = ( str.find( "//" )+2 ) ;
  size_t secondSemiColonIndex = str.find( ':', doubleSlashIndex );
  return str.substr( doubleSlashIndex, secondSemiColonIndex-doubleSlashIndex );
}

}

void tsyn::Endpoint::parse( const std::string & endp )
{
  m_protocol = parseProto( endp );
  m_ipAddress = parseIpAddress( endp );
  m_port = parsePort( endp );
}

