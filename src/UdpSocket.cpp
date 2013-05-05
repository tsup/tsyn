#include "UdpSocket.hpp"
#include "UdpConnection.hpp"

#include <boost/asio.hpp>

tsyn::UdpSocket::UdpSocket(
    boost::asio::io_service & service,
    const boost::asio::ip::udp::endpoint& localEndpoint,
    ReceiveQueue& receiveQueue,
    ConnectionTable& connections )
  : m_buffer()
  , m_localEndpoint( localEndpoint )
  , m_remoteEndpoint()
  , m_socket( service, localEndpoint )
  , m_receiveQueue( receiveQueue )
  , m_udpConnections()
  , m_connections( connections )
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
  messageArrivedTo( m_remoteEndpoint );
  startReceive();
}


void
tsyn::UdpSocket::messageArrivedTo( const boost::asio::ip::udp::endpoint & remote )
{
  const std::string endpoint(
      std::string( "udp://" ) + remote.address().to_string() + ":" + std::to_string( remote.port() ) );
  std::cout << "message arrived from: " << endpoint<< std::endl;

  auto it = m_udpConnections.find(remote);
  if ( m_udpConnections.end() == it )
  {
    UdpConnection::Ref newUdpConnection( new UdpConnection );
    auto result = m_udpConnections.emplace( remote, newUdpConnection.get() );

    m_connections.insert(
        endpoint,
        Connection::Ref( new Connection(
          LowLevelConnectionRef( std::move( newUdpConnection ) ),
          m_receiveQueue ) ) );
    it = result.first;
  }

  it->second->receive( Data( begin(m_buffer), end(m_buffer) ) );
}

