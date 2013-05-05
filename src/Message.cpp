#include "Message.hpp"

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

}

tsyn::Data tsyn::serialize( const tsyn::Message& msg )
{
  tsyn::Data message;

  uint32_t length = tsyn::Message::HEADER_LENGTH + msg.payload.length();

  extractBytesFrom(length).to(message);
  extractBytesFrom(msg.timestamp).to(message);
  extractBytesFrom(msg.type).to(message);

  message += msg.payload;
  return message;
}

