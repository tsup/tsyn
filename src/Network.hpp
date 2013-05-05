#pragma once

#include <thread>
#include <iostream>
#include <iomanip>
#include <boost/asio.hpp>

#include "TcpAcceptor.hpp"
#include "LowLevelConnection.hpp"
#include "Connection.hpp"
#include "RingBuffer.hpp"
#include "Types.hpp"

namespace tsyn
{

  class UdpPeer : public LowLevelConnection
  {
    public:
      typedef std::unique_ptr<UdpPeer> Ref;
      UdpPeer()
      {
        std::cout << "UdpPeer created!" << std::endl;
      }
      virtual void start( Connection& ownerConnection ) override
      {
        m_ownerConnection = &ownerConnection;
      }
      virtual void send( Data&& ) override
      {}

      void receive( const Data& data )
      {
        m_ownerConnection->receive( data );
      }
    private:
      Connection* m_ownerConnection;
  };

  class UdpSocket
  {
    public:
      typedef std::unique_ptr< UdpSocket > Ref;
      UdpSocket( boost::asio::io_service & service,
                 boost::asio::ip::udp::endpoint localEndpoint )
        : m_service(service)
        , m_localEndpoint(localEndpoint)
        , m_socket(service, localEndpoint)
      {
        startReceive();
      }

      void startReceive()
      {
        m_socket.async_receive_from(
            boost::asio::buffer(m_buffer),
            m_remoteEndpoint,
            std::bind( &UdpSocket::distribute, this, std::placeholders::_1 ) );
      }

      void distribute( const boost::system::error_code & )
      {
        std::cout << "UDP: Received from: "
                  << m_remoteEndpoint.address().to_string() << ":"
                  << m_remoteEndpoint.port() << std::endl;
        messageArrivedTo( m_remoteEndpoint );
        startReceive();
      }

    private:

      void messageArrivedTo( const boost::asio::ip::udp::endpoint & remote )
      {
        auto it = m_udpPeers.find(remote);
        if ( m_udpPeers.end() == it )
        {
          auto result = m_udpPeers.emplace( remote, UdpPeer::Ref(new UdpPeer) );
          it = result.first;
        }
        it->second->receive( Data( begin(m_buffer), end(m_buffer) ) );
      }
      boost::asio::io_service &      m_service;
      boost::asio::ip::udp::endpoint m_localEndpoint;
      boost::asio::ip::udp::socket   m_socket;
      boost::asio::ip::udp::endpoint m_remoteEndpoint;

      std::map< boost::asio::ip::udp::endpoint, UdpPeer::Ref > m_udpPeers;

      const static size_t BUFFER_SIZE = 1024;
      std::array< char, BUFFER_SIZE > m_buffer;
  };


  class Network
  {
    public:
      void listenTcp( int port )
      {
        boost::asio::ip::tcp::endpoint endpoint( boost::asio::ip::tcp::v4(), port );
        m_acceptors.emplace_back( new TcpAcceptor( m_service, endpoint, m_receiveQueue ) );
      }

      void listenUdp( int port )
      {
        boost::asio::ip::udp::endpoint localEndpoint( boost::asio::ip::udp::v4(), port );
        m_udpSockets.emplace_back( new UdpSocket( m_service, localEndpoint ) );
      }

      void run();
      void stop();
    private:
      std::thread                     m_networkThread;
      boost::asio::io_service         m_service;
      std::vector< TcpAcceptor::Ref > m_acceptors;
      std::vector< UdpSocket::Ref >   m_udpSockets;
      ReceiveQueue                    m_receiveQueue;
  };

}

