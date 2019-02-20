
#include <algorithm>
#include "gtest/gtest.h"
#include "SseEventEncoder.h"

namespace {

class Encoder : public SSE::EventEncoder {

public: 
    Encoder () :
        SSE::EventEncoder () 
    {
        Pipe (this, &Encoder::HandleEvent);
    }
protected:
    virtual void HandleEvent (const std::string& inEvent) {
        mEvents.push_back (inEvent);
    }
public:
    std::vector<std::string> mEvents;
};

}

TEST (SseEventEncoderTester, SimplePayload) {
    Encoder encoder;
    encoder.Write ("Simple payload");
    encoder.Write ("Simple payload");
    ASSERT_EQ ("id: 1\ndata: Simple payload\n\n", encoder.mEvents[0]);
    ASSERT_EQ ("id: 2\ndata: Simple payload\n\n", encoder.mEvents[1]);
}

TEST (SseEventEncoderTester, MultiLinePayload) {
    Encoder encoder;
    encoder.Write ("First Line\nSecond line");
    ASSERT_EQ ("id: 1\ndata: First Line\ndata: Second line\n\n", encoder.mEvents[0]);
}

TEST (SseEventEncoderTester, ResettingLastId) {
    Encoder encoder;
    encoder.Write ("Simple payload");
    encoder.SetLastId (10u);
    encoder.Write ("Simple payload");
    ASSERT_EQ ("id: 1\ndata: Simple payload\n\n", encoder.mEvents[0]);
    ASSERT_EQ ("id: 11\ndata: Simple payload\n\n", encoder.mEvents[1]);
}