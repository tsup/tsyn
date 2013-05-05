#include "UdpSocket.hpp"
#include "UdpConnection.hpp"

#include <boost/asio.hpp>

tsyn::UdpSocket::UdpSocket(
    boost::asio::io_service & service,
    const boost::asio::ip::udp::endpoint& localEndpoint,
    ReceiveQueue& receiveQueue )
  : m_localEndpoint(localEndpoint)
  , m_socket(service, localEndpoint)
  , m_receiveQueue( receiveQueue )
{
  startReceive();
}


void
tsyn::UdpSocket::startReceive()
{
  std::cout << "udp receive started\n";
  m_socket.async_receive_from(
      boost::asio::buffer(m_buffer),
      m_remoteEndpoint,
      std::bind( &UdpSocket::distribute, this, std::placeholders::_1 ) );
}


void
tsyn::UdpSocket::distribute( const boost::system::error_code & )
{
  std::cout << "UDP: Received from: "
    << m_remoteEndpoint.address().to_string() << ":"
    << m_remoteEndpoint.port() << std::endl;
  messageArrivedTo( m_remoteEndpoint );
  startReceive();
}


void
tsyn::UdpSocket::messageArrivedTo( const boost::asio::ip::udp::endpoint & remote )
{
  auto it = m_udpConnections.find(remote);
  if ( m_udpConnections.end() == it )
  {
    UdpConnection* newUdpConnection( new UdpConnection );
    auto result = m_udpConnections.emplace( remote, newUdpConnection );
    m_connections.emplace_back( new Connection(
          LowLevelConnectionRef( newUdpConnection ), m_receiveQueue ) );
    it = result.first;
  }
  it->second->receive( Data( begin(m_buffer), end(m_buffer) ) );
}

