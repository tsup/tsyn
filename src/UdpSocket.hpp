#pragma once

#include <memory>
#include <boost/asio.hpp>

#include "Types.hpp"
#include "RingBuffer.hpp"
#include "Connection.hpp"
#include "TcpConnection.hpp"

namespace tsyn
{
  class UdpConnection;

  class UdpSocket
  {
    public:
      typedef std::unique_ptr< UdpSocket > Ref;
      UdpSocket( boost::asio::io_service & service,
                 const boost::asio::ip::udp::endpoint& localEndpoint,
                 ReceiveQueue& receiveQueue,
                 ConnectionTable& connections );

    private:
      void startReceive();
      void distribute( const boost::system::error_code & );
      void messageArrivedTo( const boost::asio::ip::udp::endpoint & remote );

      typedef std::map< boost::asio::ip::udp::endpoint, UdpConnection* > UdpConnectionContainer;

      const static size_t BUFFER_SIZE = 1024;
      std::array< char, BUFFER_SIZE > m_buffer;
      boost::asio::ip::udp::endpoint  m_localEndpoint;
      boost::asio::ip::udp::endpoint  m_remoteEndpoint;
      boost::asio::ip::udp::socket    m_socket;
      ReceiveQueue&                   m_receiveQueue;
      UdpConnectionContainer          m_udpConnections;
      ConnectionTable&                m_connections;
  };

}

