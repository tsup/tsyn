#include "Network.hpp"
#include "TcpAcceptor.hpp"
#include "UdpSocket.hpp"
#include "Endpoint.hpp"
#include "TcpConnection.hpp"
#include "UdpConnection.hpp"

#include <boost/asio.hpp>
#include <thread>


tsyn::Network::Network( ReceiveQueue& receiveQueue, SendQueue& sendQueue )
  : m_receiveQueue( receiveQueue )
  , m_sendQueue( sendQueue )
{
}


void
tsyn::Network::run()
{
  m_networkThread = std::thread(
      [ this ] ()
      {
        while ( true )
        {
          m_service.poll();
          std::unique_ptr< tsyn::QueueData > message;
          while ( m_sendQueue.pop( message ) )
          {
            std::cout << "should send: " << message->payload << std::endl;
            m_connectionTable.on( message->peerId,
              [ &message ]( Connection& connection )
              {
                std::cout << "matching connection found" << std::endl;
                connection.send(
                  message->payload,
                  message->timestamp );
              });
          }
        }
      } );
}


void
tsyn::Network::stop()
{
  m_service.stop();
  if ( m_networkThread.joinable() )
  {
      m_networkThread.join();
  }
}


void
tsyn::Network::listenTcp( int port )
{
  boost::asio::ip::tcp::endpoint endpoint( boost::asio::ip::tcp::v4(), port );
  m_acceptors.emplace_back(
      new TcpAcceptor( m_service, endpoint, m_receiveQueue, m_connectionTable ) );
}


tsyn::UdpSocketRef&
tsyn::Network::createUdpSocketWithPort( int port )
{
  boost::asio::ip::udp::endpoint localEndpoint( boost::asio::ip::udp::v4(), port );
  m_udpSockets.emplace_back( new UdpSocket( m_service, localEndpoint, m_receiveQueue, m_connectionTable ) );
  return m_udpSockets.back();
}


void
tsyn::Network::listenUdp( int port )
{
  createUdpSocketWithPort( port );
}


void
tsyn::Network::connectTo( const std::string& address )
{
  tsyn::Endpoint endpoint( address );
  if ( tsyn::Proto::TCP == endpoint.protocol() )
  {
    m_connectionTable.insert(
        address,
        ConnectionRef(
          new Connection(
            tsyn::TcpConnection::connectTo( endpoint, m_service ),
            m_receiveQueue ) ) );
    return;
  }

  createUdpSocketWithPort( 0 )->connectTo( endpoint );
}

