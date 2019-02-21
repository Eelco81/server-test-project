
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
    decoder.Write ("event: some event\nid: 2\ndata: Simple payload 2\n\n");
    ASSERT_EQ ("Simple payload 1", decoder.mEvents[0]);
    ASSERT_EQ ("Simple payload 2", decoder.mEvents[1]);
}

TEST (SseEventDecoderTester, MultiLinePayload) {
    Decoder decoder;
    decoder.Write ("event: some event\nid: 1\ndata: Simple payload 1\ndata: Simple payload 2\n\n");
    decoder.Write ("event: some event\nid: 2\ndata: Simple payload 3\n\n");
    ASSERT_EQ ("Simple payload 1\nSimple payload 2", decoder.mEvents[0]);
    ASSERT_EQ ("Simple payload 3", decoder.mEvents[1]);
}

TEST (SseEventDecoderTester, MultipleMessagesOnePayload) {
    Decoder decoder;
    decoder.Write ("event: some event\nid: 1\ndata: Simple payload 1\n");
    decoder.Write ("data: Simple payload 2\n\n");
    decoder.Write ("event: some event\nid: 2\ndata: Simple payload 3\n");
    decoder.Write ("data: Simple payload 4\n\n");
    ASSERT_EQ ("Simple payload 1\nSimple payload 2", decoder.mEvents[0]);
    ASSERT_EQ ("Simple payload 3\nSimple payload 4", decoder.mEvents[1]);
}