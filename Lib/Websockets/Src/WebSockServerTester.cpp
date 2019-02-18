
#include "gmock/gmock.h"
#include "WebSockServer.h"
#include "WebSockRemoteClient.h"
#include "HttpRequest.h"
#include "Socket.h"
#include "Log.h"
#include "Timing.h"

#define IP_FOR_TESTING "127.0.0.1"
#define PORT_FOR_TESTING "1703"

class WebSockServerTester : public ::testing::Test {
    void TearDown () {
        OS::Log::Instance ().Flush ();
    }
};

TEST_F (WebSockServerTester, StringPayloads) {
    
    RFC6455::Server server (IP_FOR_TESTING, PORT_FOR_TESTING);
    
    server.Start ();
    OS::Timing::Sleep (100u);
    
    RFC6455::RemoteClient client (IP_FOR_TESTING, PORT_FOR_TESTING);
    OS::Timing::Sleep (100u);
    
    const std::string sendPayload ("HELLO");
    std::string receivedPayload;
    
    client.GetStringDecoder ().Pipe ([&] (const std::string& payload) {
        receivedPayload = payload;
    });
    
    server.BroadCast (sendPayload);
    OS::Timing::Sleep (100u);
    
    ASSERT_EQ (sendPayload, receivedPayload);
}

TEST_F (WebSockServerTester, BinaryPayloads) {
    
    RFC6455::Server server (IP_FOR_TESTING, PORT_FOR_TESTING);
    
    server.Start ();
    OS::Timing::Sleep (100u);
    
    RFC6455::RemoteClient client (IP_FOR_TESTING, PORT_FOR_TESTING);
    OS::Timing::Sleep (100u);
    
    const std::vector<uint8_t> sendPayload (10000, 0x01);
    std::vector<uint8_t> receivedPayload;
    
    client.GetBinaryDecoder ().Pipe ([&] (const std::vector<uint8_t>& payload) {
        receivedPayload = payload;
    });
    
    server.BroadCast (sendPayload);
    OS::Timing::Sleep (100u);
    
    ASSERT_TRUE (std::equal(sendPayload.begin (), sendPayload.end (), receivedPayload.begin ()));
}
