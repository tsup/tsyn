#include "Connection.hpp"

#include <limits>
#include <tsyn/Clock.hpp>

#include "Sender.hpp"

tsyn::Connection::Connection( SenderRef sender )
  : m_sender( std::move(sender) )
{
}

namespace
{

template<typename IntType>
class ByteExtractor
{
  public:
    ByteExtractor(IntType value)
      : m_value(value)
    {}

    void to( tsyn::Data & outputBuffer )
    {
      constexpr int numberOfDigits = std::numeric_limits<IntType>::digits;
      static_assert( numberOfDigits % 8 == 0, "Cannot extract from this type!" );

      for ( int i = (numberOfDigits - 8); i >= 0; i -= 8 )
      {
        outputBuffer.append( 1, (m_value >> i) & 0xff );
      }
    }
  private:
    IntType m_value;
};

template<typename IntType>
ByteExtractor<IntType> extractBytesFrom(IntType value)
{
  return ByteExtractor<IntType>(value);
}

const uint32_t HEADER_LENGTH = 9;

tsyn::Data createMessage(tsyn::Clock::Time time, uint8_t type, const tsyn::Data & payload)
{
  tsyn::Data message;

  uint32_t length = HEADER_LENGTH + payload.length();

  extractBytesFrom(length).to(message);
  extractBytesFrom(time).to(message);
  extractBytesFrom(type).to(message);

  message += payload;

  return message;
}

}

void tsyn::Connection::send( const tsyn::Data & payload, tsyn::Clock::Time timestamp )
{
  m_sender->send( createMessage(timestamp, MessageType::USER, payload) );
}

