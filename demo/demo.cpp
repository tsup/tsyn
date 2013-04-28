#include <Network.hpp>
#include <thread>
#include <chrono>

int main()
{
  tsyn::Network net;
  net.listen( 8081 );
  net.listen( 8082 );
  net.listen( 8083 );
  net.run();
  std::this_thread::sleep_for( std::chrono::seconds( 30 ) );
  net.stop();
  return 0;
}

