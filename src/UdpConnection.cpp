#include "UdpConnection.hpp"
#include "Connection.hpp"
#include "Endpoint.hpp"

#include <iostream>

tsyn::UdpConnection::UdpConnection(
    SocketRef& socket,
    const Endpoint& remote )
  : m_socket( socket )
  , m_remoteEndpoint( remote )
  , m_boostEndpoint(
      boost::asio::ip::address::from_string( m_remoteEndpoint.ipAddress() ),
      m_remoteEndpoint.port() )
{
  std::cout << "UdpConnectioncreated!" << std::endl;
}


void
tsyn::UdpConnection::start( Connection& ownerConnection )
{
  m_ownerConnection = &ownerConnection;
}


void
tsyn::UdpConnection::receive( const Data& data )
{
  m_ownerConnection->receive( data );
}


void
tsyn::UdpConnection::send( Data&& message )
{
  std::cout << "sync udp write: " << message;
  m_socket->send_to( boost::asio::buffer( message ), m_boostEndpoint );
}


const tsyn::Endpoint&
tsyn::UdpConnection::remoteEndpoint() const
{
  return m_remoteEndpoint;
}

