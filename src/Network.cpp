#include "Network.hpp"
#include "TcpAcceptor.hpp"
#include "UdpSocket.hpp"

#include <boost/asio.hpp>
#include <thread>

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
  m_networkThread.join();
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

