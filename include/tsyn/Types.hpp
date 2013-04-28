#pragma once

#include <string>
#include <memory>

#include <tsyn/Peer.hpp>
#include <tsyn/Clock.hpp>

namespace tsyn
{
  typedef std::string Data;

  struct QueueData
  {
    typedef std::unique_ptr< QueueData > Ref;
    Clock::Time timestamp;
    Peer::Id peerId;
    Data payload;
  };

}

