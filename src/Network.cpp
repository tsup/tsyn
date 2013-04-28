#include "Network.hpp"

#include <thread>

void
tsyn::Network::run()
{
  m_networkThread = std::thread(
      [ this ] ()
      {
        m_service.run();
      } );
}


void
tsyn::Network::stop()
{
  m_service.stop();
  m_networkThread.join();
}

