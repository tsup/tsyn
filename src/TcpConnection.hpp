#pragma once

#include <memory>
#include <array>
#include <boost/asio.hpp>

#include "Types.hpp"
#include "LowLevelConnection.hpp"
#include "Endpoint.hpp"

namespace tsyn
{

  class Connection;

  class TcpConnection : public LowLevelConnection
  {
    public:
      typedef std::unique_ptr<TcpConnection> Ref;
      TcpConnection( boost::asio::io_service& );
      virtual void start( Connection& connection ) override;
      virtual void send( Data&& message ) override;

      boost::asio::ip::tcp::socket& socket();
      static Ref connectTo( const Endpoint& endpoint, boost::asio::io_service& );

      virtual const tsyn::Endpoint& remoteEndpoint() const;
    private:
      TcpConnection( const boost::asio::ip::tcp::endpoint&, boost::asio::io_service& );

      void startLengthRead();
      void handleLengthRead( const boost::system::error_code& );
      void startMessageRead();
      void handleMessageRead( const boost::system::error_code& );

      boost::asio::ip::tcp::socket m_socket;
      std::array<char, 1024>       m_buffer;
      Connection*                  m_ownerConnection;
      uint32_t                     m_payloadLength;
      std::unique_ptr< Endpoint >  m_remoteEndpoint;
  };

}

