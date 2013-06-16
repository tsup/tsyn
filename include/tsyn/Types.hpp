#pragma once

#include <string>
#include <memory>
#include <ostream>

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


  inline
  std::ostream&
  operator<<( std::ostream& output, tsyn::QueueData& message )
  {
    output << "timestamp: " << message.timestamp << std::endl;
    output << "peer id: " << message.peerId << std::endl;
    output << "payload: " << message.payload<< std::endl;
    return output;
  }


}

