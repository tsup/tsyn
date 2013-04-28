#pragma once

namespace tsyn
{
  class Clock;

  class EngineImpl
  {
    public:
      EngineImpl() = delete;
      EngineImpl( const EngineImpl& ) = delete;
      EngineImpl& operator=( const EngineImpl& ) = delete;

      EngineImpl( const Clock& ) {}
  };
}

