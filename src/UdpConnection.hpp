#pragma once

#include <boost/asio.hpp>
#include <memory>

#include "LowLevelConnection.hpp"
#include "Types.hpp"
#include "Endpoint.hpp"

namespace tsyn
{
  class Endpoint;

  class UdpConnection : public LowLevelConnection
  {
    private:
      typedef boost::asio::ip::udp::socket SocketType;
      typedef std::shared_ptr< SocketType > SocketRef;

    public:
      typedef std::unique_ptr<UdpConnection> Ref;

      UdpConnection( SocketRef& socket, const Endpoint& );
      virtual void start( Connection& ownerConnection ) override;
      virtual void send( Data&& ) override;
      void receive( const Data& data );

      const Endpoint& remoteEndpoint() const;
    private:

      Connection* m_ownerConnection;
      SocketRef   m_socket;
      const Endpoint m_remoteEndpoint;
      const boost::asio::ip::udp::endpoint m_boostEndpoint;
  };
}

