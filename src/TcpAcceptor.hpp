#pragma once

#include "Connection.hpp"
#include "TcpConnection.hpp"
#include "Types.hpp"

namespace tsyn
{
  class TcpAcceptor
  {
    public:
      typedef std::unique_ptr< TcpAcceptor > Ref;
      TcpAcceptor( boost::asio::io_service& io_service,
                   const boost::asio::ip::tcp::endpoint& endpoint,
                   ReceiveQueue& receiveQueue,
                   ConnectionTable& connections );

    private:
      void startAccept();
      void handleAccept( const boost::system::error_code& error );

      boost::asio::io_service&          m_service;
      boost::asio::ip::tcp::acceptor    m_acceptor;
      TcpConnection::Ref                m_nextConnection;
      ReceiveQueue&                     m_receiveQueue;
      ConnectionTable&                  m_connections;
  };
}

