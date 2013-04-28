#pragma once

#include <tsyn/Types.hpp>

namespace tsyn
{
  class EngineImpl;

  class Engine
  {
    public:
      Engine() = delete;
      Engine( const Engine& ) = delete;
      Engine& operator=( const Engine& ) = delete;

      Engine( const Clock& clock );

      bool receive( QueueData::Ref& queueData );
      bool send( const Peer::Id& peerId, Data&& payload );
      void connectTo( const std::string& address );
      void listenOn( const std::string& address );
      void start();

    private:
      typedef std::unique_ptr< EngineImpl > EngineImplRef;
      EngineImplRef m_pimpl;
  };

}

