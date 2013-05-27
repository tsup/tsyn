#include "Endpoint.hpp"

#include <algorithm>
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
void checkStringIndex( size_t index )
{
  if ( std::string::npos == index )
    throw std::system_error();
}

void forEachToken( const std::string & in,
                   char delimiter,
                   std::function<void(const std::string&)> func )
{
  size_t next = 0;
  size_t prev = 0;

  while( next != std::string::npos )
  {
    next = in.find(delimiter,next+1);
    func(in.substr(prev,next-prev));
    prev = next+1;
  }
}

void checkValidIp( const std::string & ip )
{
    const char delimiter = '.';

    if ( std::count(ip.begin(), ip.end(), delimiter) != 3 )
      throw std::system_error();

    forEachToken( ip, delimiter, []( const std::string & token )
    {
      const int element = std::stoi(token);
      if ( element < 0 || element > 255 )
        throw std::system_error();
    });
}

tsyn::Proto parseProto( const std::string & str )
{
  const size_t semiColonIndex = str.find( ':' );
  checkStringIndex(semiColonIndex);
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
  try
  {
    const size_t lastSemiColon = str.find_last_of( ':' );
    checkStringIndex(lastSemiColon);
    const int port = std::stoi( str.substr( lastSemiColon+1 ) );
    if ( port <= 0 || port >= 65536 )
      throw std::system_error();
    return port;
  }
  catch(const std::invalid_argument & ex) { throw std::system_error(); }
  catch(const std::out_of_range & ex) { throw std::system_error(); }
}

const std::string parseIpAddress( const std::string & str )
{
  const size_t doubleSlashIndex = ( str.find( "//" )+2 ) ;
  checkStringIndex(doubleSlashIndex);

  const size_t secondSemiColonIndex = str.find( ':', doubleSlashIndex );
  checkStringIndex(secondSemiColonIndex);

  const std::string ret = str.substr( doubleSlashIndex, secondSemiColonIndex-doubleSlashIndex );
  checkValidIp(ret);
  return ret;
}

}

void tsyn::Endpoint::parse( const std::string & endp )
{
  m_protocol = parseProto( endp );
  m_ipAddress = parseIpAddress( endp );
  m_port = parsePort( endp );
}

