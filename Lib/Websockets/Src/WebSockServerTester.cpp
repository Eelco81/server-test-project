
#include "gmock/gmock.h"
#include "WebSockServer.h"
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

TEST_F (WebSockServerTester, BadRequest) {
    
    RFC6455::Server server (IP_FOR_TESTING, PORT_FOR_TESTING);
    
    server.Start ();
    OS::Timing::Sleep (100u);

    OS::Socket client (IP_FOR_TESTING, PORT_FOR_TESTING);
    client.Initialize ();
    ASSERT_TRUE (client.Connect ());

    HTTP::Request request (HTTP::Method::GET, "/hello");
    auto strReq (request.ToString ());
    client.Send (reinterpret_cast<const uint8_t*> (strReq.c_str ()), strReq.size ());
    
    std::vector<uint8_t> buffer (100u);
    ASSERT_LE (0, client.Receive (buffer)); // blocking call
    
    std::string strRes;
    strRes.assign (reinterpret_cast<char*> (buffer.data ()));
    ASSERT_THAT (strRes, ::testing::HasSubstr ("HTTP/1.1 400 Bad Request"));
    
    client.Close ();
    server.Stop ();
}

TEST_F (WebSockServerTester, Handshake) {
    
    RFC6455::Server server (IP_FOR_TESTING, PORT_FOR_TESTING);
    
    server.Start ();
    OS::Timing::Sleep (100u);

    OS::Socket client (IP_FOR_TESTING, PORT_FOR_TESTING);
    client.Initialize ();
    ASSERT_TRUE (client.Connect ());

    HTTP::Request request (HTTP::Method::GET, "/hello");
    request.mHeaders["Connection"] = "Upgrade";
    request.mHeaders["Upgrade"] = "websocket";
    request.mHeaders["Sec-WebSocket-Key"] = "key";
    
    auto strReq (request.ToString ());
    client.Send (reinterpret_cast<const uint8_t*> (strReq.c_str ()), strReq.size ());
    
    std::vector<uint8_t> buffer (100u);
    ASSERT_LE (0, client.Receive (buffer)); // blocking call
    
    std::string strRes;
    strRes.assign (reinterpret_cast<char*> (buffer.data ()));
    ASSERT_THAT (strRes, ::testing::HasSubstr ("HTTP/1.1 101 Switching Protocols"));
    
    client.Close ();
    server.Stop ();
}
