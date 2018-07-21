
#include "gtest/gtest.h"

#include "Buffer.h"
#include "Timing.h"
#include "Log.h"
#include "Network.h"
#include "Socket.h"
#include "TcpServer.h"
#include "TcpClientFactory.h"
#include "TcpClient.h"

#define IP_FOR_TESTING "127.0.0.1"
#define PORT_FOR_TESTING "1703"

namespace {

class EchoClient : public TCP::Client {

public:
    EchoClient(std::unique_ptr<OS::Socket> inSocket) :
       TCP::Client (std::move (inSocket)) 
    {
    }
    void OnReceived (const OS::Buffer& inBuffer) override {
        Send (inBuffer);
    }
};
    
class TestClientFactory : public TCP::ClientFactory {
    
    
public:
    std::unique_ptr<TCP::Client> Create (std::unique_ptr<OS::Socket> inSocket) override {
        return std::make_unique<EchoClient> (std::move (inSocket));
    }
    
};
    
}

class TcpServerTester : public ::testing::Test {
    void SetUp () {
        OS::Log::Instance ().Initialize (OS::Log::kTrace);
        OS::Network::Initialize ();
    }
    void TearDown () {
        OS::Network::Done ();
        OS::Log::Instance ().Flush ();
    }
};

TEST_F (TcpServerTester, OpenAndCloseConnection) {
    
    const char* kTestData = "Some Other Data";
    const unsigned kTestSize (static_cast<unsigned> (strlen (kTestData)));
    
    auto factory (std::make_unique<TestClientFactory>());
    TCP::Server server (IP_FOR_TESTING, PORT_FOR_TESTING, std::move (factory));
    
    server.Start();
    OS::Timing::Sleep (100);
    
    OS::Socket socket(IP_FOR_TESTING, PORT_FOR_TESTING);
    socket.Initialize();
    EXPECT_TRUE (socket.Connect());
    
    OS::Timing::Sleep (100);
    
    OS::Buffer bufferSend (20u);
    bufferSend.SetData (kTestData, kTestSize);    
    socket.Send (bufferSend);

    OS::Buffer bufferReceive (20u);
    socket.Receive (bufferReceive);
    
    std::string bufferString;
    bufferReceive.ToString (bufferString);
    EXPECT_EQ (std::string (kTestData), bufferString);
    
    socket.Close ();
    server.Stop ();
}

TEST_F (TcpServerTester, MultipleConnnections) {
    
    std::vector<std::unique_ptr<OS::Socket>> sockets;
    for (std::size_t i (0u); i < 5u; ++i) {
        sockets.emplace_back (std::make_unique<OS::Socket> (IP_FOR_TESTING, PORT_FOR_TESTING));
    }
    
    auto factory (std::make_unique<TestClientFactory>());
    TCP::Server server (IP_FOR_TESTING, PORT_FOR_TESTING, std::move (factory));
    
    server.Start();
    
    for (auto& socket : sockets) {
        socket->Initialize ();
    }
    
    OS::Timing::Sleep (100u);
    EXPECT_EQ (0u, server.GetClientCount ());
    
    for (auto& socket : sockets) {
        EXPECT_TRUE (socket->Connect ());
        EXPECT_TRUE (socket->IsConnected ());
    }
    
    OS::Timing::Sleep (100u);
    EXPECT_EQ (sockets.size (), server.GetClientCount ());
    
    for (auto& socket : sockets) {
        socket->Close ();
        EXPECT_FALSE (socket->IsConnected ());
    }
    
    OS::Timing::Sleep (2000u); // Make sure cleaner ticked once
    EXPECT_EQ (0u, server.GetClientCount ());
    
    server.Stop();
}

TEST_F (TcpServerTester, ClosingServer) {
    
    std::vector<std::unique_ptr<OS::Socket>> sockets;
    for (std::size_t i (0u); i < 5u; ++i) {
        sockets.emplace_back (std::make_unique<OS::Socket> (IP_FOR_TESTING, PORT_FOR_TESTING));
    }
    
    auto factory (std::make_unique<TestClientFactory>());
    TCP::Server server (IP_FOR_TESTING, PORT_FOR_TESTING, std::move (factory));
    
    server.Start();
    
    for (auto& socket : sockets) {
        socket->Initialize ();
    }
    
    OS::Timing::Sleep (100u);
    EXPECT_EQ (0u, server.GetClientCount ());
    
    for (auto& socket : sockets) {
        EXPECT_TRUE (socket->Connect ());
        EXPECT_TRUE (socket->IsConnected ());
    }
    
    OS::Timing::Sleep (100u);
    EXPECT_EQ (sockets.size (), server.GetClientCount ());
    
    server.Stop();
    OS::Timing::Sleep (100u);
    
    for (auto& socket : sockets) {
        OS::Buffer buffer (1u);
        EXPECT_FALSE (socket->Receive (buffer));
        EXPECT_FALSE (socket->IsConnected ());
    }
    EXPECT_EQ (0u, server.GetClientCount ());
}