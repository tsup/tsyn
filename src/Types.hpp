#pragma once

#include <string>
#include <memory>

namespace tsyn
{
  typedef std::string Data;

  class Sender;
  typedef std::unique_ptr< Sender > SenderRef;
}

