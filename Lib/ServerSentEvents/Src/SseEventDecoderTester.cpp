
#include <algorithm>
#include "gtest/gtest.h"
#include "SseEventDecoder.h"

namespace {

class Decoder : public SSE::EventDecoder {

public: 
    Decoder () :
        SSE::EventDecoder () 
    {
        Pipe (this, &Decoder::HandleEvent);
    }
protected:
    virtual void HandleEvent (const std::string& inEvent) {
        mEvents.push_back (inEvent);
    }
public:
    std::vector<std::string> mEvents;
};

}

TEST (SseEventDecoderTester, SimplePayload) {
    Decoder decoder;
    decoder.Write ("event: some event\nid: 1\ndata: Simple payload 1\n\n");
    decoder.Write ("event: some event\nid: 1\ndata: Simple payload 2\n\n");
    ASSERT_EQ ("Simple payload 1\n", decoder.mEvents[0]);
    ASSERT_EQ ("Simple payload 2\n", decoder.mEvents[1]);
}
