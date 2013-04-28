#pragma once

#include <string>

#include <tsyn/Peer.hpp>
#include <tsyn/Clock.hpp>

namespace tsyn
{
  typedef std::string Data;

  struct ReceiveQueueData
  {
    Clock::Time timestamp;
    Peer::Id peerId;
    Data payload;
  };
}
