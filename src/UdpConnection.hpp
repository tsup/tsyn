#pragma once

#include "LowLevelConnection.hpp"

namespace tsyn
{

  class UdpConnection : public LowLevelConnection
  {
    public:
      typedef std::unique_ptr<UdpConnection> Ref;
      UdpConnection();
      virtual void start( Connection& ownerConnection ) override;
      virtual void send( Data&& ) override;
      void receive( const Data& data );

    private:
      Connection* m_ownerConnection;
  };
}

