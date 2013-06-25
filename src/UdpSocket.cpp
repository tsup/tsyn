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
  , m_socket( std::make_shared< SocketType >( service, localEndpoint ) )
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
  m_socket->async_receive_from(
      boost::asio::buffer(m_buffer),
      m_remoteEndpoint,
      std::bind( &UdpSocket::distribute, this, std::placeholders::_1 ) );
}


void
tsyn::UdpSocket::distribute( const boost::system::error_code & )
{
  messageArrivedFrom( m_remoteEndpoint );
  startReceive();
}


tsyn::UdpConnection&
tsyn::UdpSocket::createConnection( const Endpoint& endpoint )
{
  const std::string remoteId( endpoint.asStr() );
  std::cout << "creating udp connection: " << remoteId << std::endl;

  auto it = m_udpConnections.find( remoteId );
  if ( m_udpConnections.end() == it )
  {
    UdpConnection::Ref newUdpConnection( new UdpConnection( m_socket, endpoint ) );
    auto result = m_udpConnections.emplace( remoteId, newUdpConnection.get() );

    m_connections.insert(
        remoteId,
        Connection::Ref( new Connection(
          LowLevelConnectionRef( std::move( newUdpConnection ) ),
          m_receiveQueue ) ) );
    it = result.first;
  }

  return *it->second;
}


void
tsyn::UdpSocket::messageArrivedFrom( const boost::asio::ip::udp::endpoint & remote )
{
  UdpConnection& connection( createConnection( endpointFromBoost( remote ) ) );
  connection.receive( Data( begin(m_buffer), end(m_buffer) ) );
}


void
tsyn::UdpSocket::connectTo( const Endpoint& endpoint )
{
  createConnection( endpoint );
}

