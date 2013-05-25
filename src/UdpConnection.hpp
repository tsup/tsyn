#pragma once

#include <boost/asio.hpp>

#include "LowLevelConnection.hpp"
#include "Types.hpp"

namespace tsyn
{
  class Endpoint;

  class UdpConnection : public LowLevelConnection
  {
    public:
      typedef std::unique_ptr<UdpConnection> Ref;
      UdpConnection();
      virtual void start( Connection& ownerConnection ) override;
      virtual void send( Data&& ) override;
      void receive( const Data& data );

      static ConnectionRef connectTo( const Endpoint&, boost::asio::io_service& );

    private:
      Connection* m_ownerConnection;
  };
}

