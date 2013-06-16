#include "Network.hpp"
#include "TcpAcceptor.hpp"
#include "UdpSocket.hpp"
#include "Endpoint.hpp"
#include "TcpConnection.hpp"
#include "UdpConnection.hpp"

#include <boost/asio.hpp>
#include <thread>


tsyn::Network::Network( ReceiveQueue& receiveQueue )
  : m_receiveQueue( receiveQueue )
{
}


void
tsyn::Network::run()
{
  m_networkThread = std::thread(
      [ this ] ()
      {
        m_service.run();
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
  m_acceptors.emplace_back( new TcpAcceptor( m_service, endpoint, m_receiveQueue, m_connectionTable ) );
}

void
tsyn::Network::listenUdp( int port )
{
  boost::asio::ip::udp::endpoint localEndpoint( boost::asio::ip::udp::v4(), port );
  m_udpSockets.emplace_back( new UdpSocket( m_service, localEndpoint, m_receiveQueue, m_connectionTable ) );
}

namespace
{
  tsyn::LowLevelConnectionRef createLowLevelConnection(
      const tsyn::Endpoint& endpoint,
      boost::asio::io_service& ioservice )
  {
    if ( tsyn::Proto::TCP == endpoint.protocol() )
    {
      return tsyn::TcpConnection::connectTo( endpoint, ioservice );
    }

    return tsyn::UdpConnection::connectTo( endpoint, ioservice );
  }
}

void
tsyn::Network::connectTo( const std::string& address )
{
  tsyn::Endpoint endpoint( address );
  m_connectionTable.insert(
      address,
      ConnectionRef( new Connection(
          createLowLevelConnection( endpoint, m_service ),
          m_receiveQueue ) ) );
}

