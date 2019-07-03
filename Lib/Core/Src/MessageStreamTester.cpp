
#include <string>

#include "gtest/gtest.h"
#include "MessageStream.h"

namespace {

class Stream : public OS::MessageStream<std::vector<char>,std::string> {
public:
    void Write (const std::vector<char>& inMessage) override {
        std::string data;
        data.assign (inMessage.data (), inMessage.data () + inMessage.size ());
        sNext.Emit (data);
    }
};

class RevStream : public OS::MessageStream<std::string,std::vector<char>> {
public:
    void Write (const std::string& inMessage) override {
        std::vector<char> data;
        data.assign (inMessage.data (), inMessage.data () + inMessage.size ());
        sNext.Emit (data);
    }
};

class ConvStream : public OS::MessageStream<std::tuple<const char*, std::size_t>, std::vector<char>> {
public:
    void Write (const std::tuple<const char*, std::size_t>& inMessage) override {
        std::vector<char> data;
        data.assign (std::get<0> (inMessage), std::get<0> (inMessage) + std::get<1> (inMessage));
        sNext.Emit (data);
    }
};

} // end anonymous namespace

TEST (MessageStreamTester, PipeToLambda) {
    
    const std::vector<char> source = { 'h', 'e', 'l', 'l', 'o' };
    std::string target;
    
    auto callback = [&](const std::string& newdata) {
        target = newdata;
    };
    
    Stream stream;
    stream.Pipe (callback);
    stream.Write (source);
    
    ASSERT_EQ (std::string ("hello"), target.data ());
}

TEST (MessageStreamTester, PipeToObject) {
    
    struct StreamObject {
        void Save (const std::string& inMessage) {
            mPacket = inMessage;
        }
        std::string mPacket;
    };

    const std::vector<char> source = { 'h', 'e', 'l', 'l', 'o' };
    
    StreamObject object;

    Stream stream;
    stream.Pipe (&object, &StreamObject::Save);
    
    stream.Write (source);
    
    ASSERT_EQ (std::string ("hello"), object.mPacket);
}

TEST (MessageStreamTester, PipeToStream) {
    
    const std::string source = "hello";
    std::string target;
    
    auto callback = [&](const std::string& newdata) {
        target = newdata;
    };
    
    RevStream revStream;
    Stream stream;
    
    revStream.Pipe (stream).Pipe (callback);
    
    revStream.Write (source);
    
    ASSERT_EQ (std::string ("hello"), target);
}


TEST(MessageStreamTester, PipeSignalToStream) {
    
    std::string target;

    auto callback = [&](const std::string& newdata) {
        target = newdata;
    };

    
    OS::Signal<const char*, std::size_t> signal;
    ConvStream convStream;
    RevStream revStream;
    Stream stream;

    Pipe (signal, convStream).Pipe (stream).Pipe (callback);

    signal.Emit ("hello", 5u);

    ASSERT_EQ(std::string("hello"), target);
}
