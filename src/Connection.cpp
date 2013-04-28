#include "Connection.hpp"

#include <tsyn/Clock.hpp>

#include "Sender.hpp"

tsyn::Connection::Connection( SenderRef sender )
  : m_sender( std::move(sender) )
{
}

