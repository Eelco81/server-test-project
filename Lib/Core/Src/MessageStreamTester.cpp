
#include "gtest/gtest.h"
#include "MessageStream.h"

namespace {
    
class Stream : public OS::MessageStream {
public:
    void Write (const OS::MessageStream::Packet& inMessage) override {
        Done (inMessage);
    }
};

struct StreamObject {
    void Save (const OS::MessageStream::Packet& inMessage) {
        mPacket = inMessage;
    }
    OS::MessageStream::Packet mPacket;
};

} // end anonymous namespace

TEST (MessageStreamTester, StreamWithLambda) {
    
    const OS::MessageStream::Packet source (25u, 0xFE);
    OS::MessageStream::Packet target;
    
    auto callback = [&](const OS::MessageStream::Packet& inPacket) {
        target = inPacket;
    };
    
    Stream stream;
    stream.AddCallback (callback);
    stream.Write (source);
    
    ASSERT_TRUE (std::equal (source.begin (), source.end (), target.begin ()));
}

TEST (MessageStreamTester, StreamWithObject) {
    
    const OS::MessageStream::Packet source (25u, 0xFE);
    
    StreamObject object;
    auto callback = std::bind (&StreamObject::Save, &object, std::placeholders::_1);
    
    callback (source);
    
    Stream stream;
    stream.AddCallback (callback);
    stream.Write (source);
    
    ASSERT_TRUE (std::equal (source.begin (), source.end (), object.mPacket.begin ()));
}
