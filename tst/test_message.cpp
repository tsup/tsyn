#include <igloo/igloo_alt.h>
using namespace igloo;

#include <Message.hpp>

#include "helpers.hpp"

Describe(AMessage)
{
  tsyn::Clock::Time arbitraryTimeStamp = 87123645;
  tsyn::Data somePayLoad = "asdklfjhalskiurhlgueh";
  tsyn::Message msg = { arbitraryTimeStamp,
                        tsyn::MessageType::USER,
                        somePayLoad };

  It(can_be_serialized)
  {
    tsyn::Data expectedSentBytes = generateUserMessage( somePayLoad, arbitraryTimeStamp );
    tsyn::Data serializedMessage = tsyn::serialize( msg );

    AssertThat( serializedMessage, Equals(expectedSentBytes) );
  }
};
