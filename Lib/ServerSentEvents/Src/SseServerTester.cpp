
#include "gmock/gmock.h"
#include "SseServer.h"
#include "SseRemoteClient.h"
#include "HttpRemoteClient.h"
#include "HttpRequest.h"
#include "Socket.h"
#include "Log.h"
#include "Timing.h"

#define IP_FOR_TESTING "127.0.0.1"
#define PORT_FOR_TESTING "1703"
#define TESTING_WAIT 50000u

class SseServerTester : public ::testing::Test {

    void TearDown () {
        OS::Log::Instance ().Flush ();
    }
    
protected:

    void VerifyBadRequest (const HTTP::Request& inRequest) {
        
        SSE::Server server (IP_FOR_TESTING, PORT_FOR_TESTING);
        
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

TEST_F (SseServerTester, StringPayloads) {
    
    SSE::Server server (IP_FOR_TESTING, PORT_FOR_TESTING);
    
    server.Start ();
    OS::Timing::Sleep (TESTING_WAIT);
    
    SSE::RemoteClient client (IP_FOR_TESTING, PORT_FOR_TESTING);
    OS::Timing::Sleep (TESTING_WAIT);
    
    const std::string sendPayload ("HELLO");
    std::string receivedPayload;
    
    client.GetEventDecoder ().Pipe ([&] (const std::string& payload) {
        receivedPayload = payload;
    });
    
    server.BroadCast (sendPayload);
    OS::Timing::Sleep (TESTING_WAIT);
    
    ASSERT_EQ (sendPayload, receivedPayload);
}


TEST_F (SseServerTester, FailedHandshakes_NoGet) {
    
    HTTP::Request request (HTTP::Method::PUT, "/");
    request.mHeaders["Accept"] = "text/event-stream";
    VerifyBadRequest (request);
}

TEST_F (SseServerTester, FailedHandshakes_WrongAccept) {
    
    HTTP::Request request (HTTP::Method::GET, "/");
    request.mHeaders["Accept"] = "no-an-event-stream";
    VerifyBadRequest (request);
}

TEST_F (SseServerTester, FailedHandshakes_NoAccept) { 

    HTTP::Request request (HTTP::Method::GET, "/");
    VerifyBadRequest (request);
}

TEST_F (SseServerTester, FailedHandshakes_WrongVersion) {
    
    HTTP::Request request (HTTP::Method::GET, "/", HTTP::Version::V10);
    request.mHeaders["Accept"] = "text/event-stream";
    VerifyBadRequest (request);
}
