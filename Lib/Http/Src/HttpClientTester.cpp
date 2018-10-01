
#include "gmock/gmock.h"

#include "Timing.h"
#include "Log.h"
#include "HttpClient.h"
#include "HttpRequest.h"
#include "HttpRouter.h"
#include "TcpServer.h"
#include "Socket.h"

#define IP_FOR_TESTING "127.0.0.1"
#define PORT_FOR_TESTING "1703"

namespace {

} // end anonymous namespace

class HttpClientTester : public ::testing::Test {
    void TearDown () {
        OS::Log::Instance ().Flush ();
    }
};

TEST_F (HttpClientTester, VerifyProperIntegration) {
    
    auto router (std::make_shared<HTTP::Router> ());
    auto factory (std::make_shared<HTTP::ClientFactory> (router));
    TCP::Server server (IP_FOR_TESTING, PORT_FOR_TESTING, factory);
    
    server.Start();
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
    ASSERT_THAT (strRes, ::testing::HasSubstr ("HTTP/1.1 404 Not Found"));
    
    client.Close ();
    server.Stop ();
}
