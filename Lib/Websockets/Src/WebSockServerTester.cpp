
#include "gmock/gmock.h"
#include "WebSockServer.h"
#include "WebSockRemoteClient.h"
#include "HttpRemoteClient.h"
#include "HttpRequest.h"
#include "Socket.h"
#include "Log.h"
#include "Timing.h"

#define IP_FOR_TESTING "127.0.0.1"
#define PORT_FOR_TESTING "1703"
#define TESTING_WAIT 100000u

class WebSockServerTester : public ::testing::Test {

    void TearDown () {
        OS::Log::Instance ().Flush ();
    }
    
protected:

    void VerifyBadRequest (const HTTP::Request& inRequest) {
        
        RFC6455::Server server (IP_FOR_TESTING, PORT_FOR_TESTING);
        
        server.Start ();
        OS::Timing::Sleep (TESTING_WAIT);
        
        HTTP::RemoteClient client (IP_FOR_TESTING, PORT_FOR_TESTING);
        OS::Timing::Sleep (TESTING_WAIT);
        
        auto response = client.Send (inRequest);
        
        ASSERT_EQ (HTTP::Code::BAD_REQUEST, response.mCode);
        
        server.Stop ();
        client.Stop ();
        
        OS::Timing::Sleep (TESTING_WAIT);
    }

};

TEST_F (WebSockServerTester, StringPayloads) {
    
    RFC6455::Server server (IP_FOR_TESTING, PORT_FOR_TESTING);
    
    server.Start ();
    OS::Timing::Sleep (TESTING_WAIT);
    
    RFC6455::RemoteClient client (IP_FOR_TESTING, PORT_FOR_TESTING);
    OS::Timing::Sleep (TESTING_WAIT);
    
    const std::string sendPayload ("HELLO");
    std::string receivedPayload;
    
    client.GetStringDecoder ().Pipe ([&] (const std::string& payload) {
        receivedPayload = payload;
    });
    
    server.BroadCast (sendPayload);
    OS::Timing::Sleep (TESTING_WAIT);
    
    ASSERT_EQ (sendPayload, receivedPayload);
}

TEST_F (WebSockServerTester, BinaryPayloads) {
    
    RFC6455::Server server (IP_FOR_TESTING, PORT_FOR_TESTING);
    
    server.Start ();
    OS::Timing::Sleep (TESTING_WAIT);
    
    RFC6455::RemoteClient client (IP_FOR_TESTING, PORT_FOR_TESTING);
    OS::Timing::Sleep (TESTING_WAIT);
    
    const std::vector<uint8_t> sendPayload (10000, 0x01);
    std::vector<uint8_t> receivedPayload;
    
    client.GetBinaryDecoder ().Pipe ([&] (const std::vector<uint8_t>& payload) {
        receivedPayload = payload;
    });
    
    server.BroadCast (sendPayload);
    OS::Timing::Sleep (TESTING_WAIT);
    
    ASSERT_TRUE (std::equal(sendPayload.begin (), sendPayload.end (), receivedPayload.begin ()));
}

TEST_F (WebSockServerTester, FailedHandshakes_NoGet) {
    
    HTTP::Request request (HTTP::Method::PUT, "/");
    request.SetHeader (HTTP::Header ("Connection", "Upgrade"));
    request.SetHeader (HTTP::Header ("Upgrade", "websocket"));
    request.SetHeader (HTTP::Header ("Sec-WebSocket-Key", "dGhlIHNhbXBsZSBub25jZQ=="));
    VerifyBadRequest (request);
}

TEST_F (WebSockServerTester, FailedHandshakes_WrongUpgrade) {
    
    HTTP::Request request (HTTP::Method::GET, "/");
    request.SetHeader (HTTP::Header ("Connection", "ThisIsNotUpgrade"));
    request.SetHeader (HTTP::Header ("Upgrade", "websocket"));
    request.SetHeader (HTTP::Header ("Sec-WebSocket-Key", "dGhlIHNhbXBsZSBub25jZQ=="));
    VerifyBadRequest (request);
}

TEST_F (WebSockServerTester, FailedHandshakes_NoUpgrade) { 

    HTTP::Request request (HTTP::Method::GET, "/");
    request.SetHeader (HTTP::Header ("Upgrade", "websocket"));
    request.SetHeader (HTTP::Header ("Sec-WebSocket-Key", "dGhlIHNhbXBsZSBub25jZQ=="));
    VerifyBadRequest (request);
}

TEST_F (WebSockServerTester, FailedHandshakes_WrongWebSocket) {
    
    HTTP::Request request (HTTP::Method::GET, "/");
    request.SetHeader (HTTP::Header ("Connection", "Upgrade"));
    request.SetHeader (HTTP::Header ("Upgrade", "NotWebsocket"));
    request.SetHeader (HTTP::Header ("Sec-WebSocket-Key", "dGhlIHNhbXBsZSBub25jZQ=="));
    VerifyBadRequest (request);
}

TEST_F (WebSockServerTester, FailedHandshakes_NoWebSocket) {
    
    HTTP::Request request (HTTP::Method::GET, "/");
    request.SetHeader (HTTP::Header ("Connection", "Upgrade"));
    request.SetHeader (HTTP::Header ("Sec-WebSocket-Key", "dGhlIHNhbXBsZSBub25jZQ=="));
    VerifyBadRequest (request);
}

TEST_F (WebSockServerTester, FailedHandshakes_NoKey) {
    
    HTTP::Request request (HTTP::Method::GET, "/");
    request.SetHeader (HTTP::Header ("Connection", "Upgrade"));
    request.SetHeader (HTTP::Header ("Upgrade", "websocket"));
    VerifyBadRequest (request);
}
