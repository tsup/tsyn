#pragma once

#include <memory>
#include <boost/asio.hpp>

#include "Types.hpp"
#include "RingBuffer.hpp"
#include "Connection.hpp"

namespace tsyn
{
  class UdpConnection;

  class UdpSocket
  {
    public:
      typedef std::unique_ptr< UdpSocket > Ref;
      UdpSocket( boost::asio::io_service & service,
                 const boost::asio::ip::udp::endpoint& localEndpoint,
                 ReceiveQueue& receiveQueue );

    private:
      void startReceive();
      void distribute( const boost::system::error_code & );
      void messageArrivedTo( const boost::asio::ip::udp::endpoint & remote );

      boost::asio::ip::udp::endpoint m_localEndpoint;
      boost::asio::ip::udp::endpoint m_remoteEndpoint;
      boost::asio::ip::udp::socket   m_socket;

      std::map< boost::asio::ip::udp::endpoint, UdpConnection* > m_udpConnections;
      std::vector< Connection::Ref > m_connections;

      const static size_t BUFFER_SIZE = 1024;
      std::array< char, BUFFER_SIZE > m_buffer;
      ReceiveQueue& m_receiveQueue;
  };

}

