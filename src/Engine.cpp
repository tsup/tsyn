#include <tsyn/Engine.hpp>
#include "EngineImpl.hpp"

tsyn::Engine::Engine( const Clock& clock )
  : m_pimpl( new EngineImpl( clock ) )
{
}


bool
tsyn::Engine::receive( QueueData::Ref& )
{
  return true;
}


bool
tsyn::Engine::send( const Peer::Id&, Data&& )
{
  return true;
}


void
connectTo( const std::string& )
{
}


void
listenOn( const std::string& )
{
}


void
start()
{
}

