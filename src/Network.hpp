#pragma once

#include <thread>
#include <boost/asio.hpp>

#include "RingBuffer.hpp"
#include "Types.hpp"
#include "TcpAcceptor.hpp"
#include "UdpSocket.hpp"
#include "PeerIndexedContainer.hpp"

namespace tsyn
{

  class Network
  {
    public:
      Network( ReceiveQueue& receiveQueue );
      void listenTcp( int port );
      void listenUdp( int port );
      void connectTo( const std::string& address );
      void run();
      void stop();

    private:
      std::thread                     m_networkThread;
      boost::asio::io_service         m_service;
      std::vector< TcpAcceptorRef >   m_acceptors;
      std::vector< UdpSocketRef >     m_udpSockets;
      ReceiveQueue&                   m_receiveQueue;
      ConnectionTable                 m_connectionTable;
  };

}

