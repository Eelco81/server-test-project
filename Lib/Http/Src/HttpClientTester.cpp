
#include "gmock/gmock.h"

#include "Timing.h"
#include "Log.h"
#include "HttpClient.h"
#include "HttpRemoteClient.h"
#include "HttpRequest.h"
#include "HttpRouter.h"
#include "TcpServer.h"
#include "Socket.h"

#define IP_FOR_TESTING "127.0.0.1"
#define PORT_FOR_TESTING "1703"

class HttpClientTester : public ::testing::Test {
    void TearDown () {
        OS::Log::Instance ().Flush ();
    }
};

TEST_F (HttpClientTester, VerifyProperIntegration) {
    
    HTTP::Router router;
    auto factory (std::make_shared<HTTP::ClientFactory> (router));
    TCP::Server server (IP_FOR_TESTING, PORT_FOR_TESTING, factory);
    
    server.Start ();
    OS::Timing::Sleep (100000u);
    
    HTTP::RemoteClient client (IP_FOR_TESTING, PORT_FOR_TESTING);
    HTTP::Request request (HTTP::Method::GET, "/hello");
    
    auto response = client.Send (request);
    
    ASSERT_EQ (response.mCode, HTTP::Code::NOT_FOUND);

    // Server and client stopped in destructor
}
