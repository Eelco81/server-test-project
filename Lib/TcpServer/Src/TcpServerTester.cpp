
#include "gtest/gtest.h"

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
    EchoClient (std::unique_ptr<OS::Socket> inSocket) :
       TCP::Client (std::move (inSocket)) 
    {
    }
    void OnReceived (const std::vector<uint8_t>& inBuffer) override {
        Send (inBuffer);
    }
};

class TestClientFactory : public TCP::ClientFactory {
public:
    std::unique_ptr<TCP::Client> Create (std::unique_ptr<OS::Socket> inSocket) const override {
        return std::make_unique<EchoClient> (std::move (inSocket));
    }
};

} // end anonymous namespace

class TcpServerTester : public ::testing::Test {
    void SetUp () {
        OS::Log::Instance ().Initialize (OS::Log::kTrace);    }
    void TearDown () {
        OS::Log::Instance ().Flush ();
    }
};

TEST_F (TcpServerTester, OpenAndCloseConnection) {
    
    const int kBufferSize (1000);
    auto factory (std::make_unique<TestClientFactory>());
    TCP::Server server (IP_FOR_TESTING, PORT_FOR_TESTING, std::move (factory));
    
    server.Start();
    OS::Timing::Sleep (100);
    
    OS::Socket socket(IP_FOR_TESTING, PORT_FOR_TESTING);
    socket.Initialize();
    EXPECT_TRUE (socket.Connect());
    
    OS::Timing::Sleep (100);
    
    const std::vector<uint8_t> bufferSend (kBufferSize, 0xFF);
    EXPECT_EQ (socket.Send (bufferSend), kBufferSize);

    std::vector<uint8_t> bufferReceive (kBufferSize + 1u, 0x00);
    EXPECT_EQ (socket.Receive (bufferReceive), kBufferSize);
    
    EXPECT_TRUE (std::equal (bufferSend.begin(), bufferSend.end(), bufferReceive.begin()));
    
    socket.Close ();
    server.Stop ();
}

TEST_F (TcpServerTester, MultipleConnnections) {
    
    auto factory (std::make_unique<TestClientFactory>());
    TCP::Server server (IP_FOR_TESTING, PORT_FOR_TESTING, std::move (factory));
    
    server.Start();
    
    OS::Timing::Sleep (100u);
    EXPECT_EQ (0u, server.GetClientCount ());
    
    std::vector<std::unique_ptr<OS::Socket>> sockets;
    for (std::size_t i (0u); i < 5u; ++i) {
        sockets.emplace_back (std::make_unique<OS::Socket> (IP_FOR_TESTING, PORT_FOR_TESTING));
        sockets.back()->Initialize();
        EXPECT_TRUE (sockets.back()->Connect ());
        EXPECT_TRUE (sockets.back()->IsConnected ());
    }
    
    OS::Timing::Sleep (100u);
    EXPECT_EQ (sockets.size (), server.GetClientCount ());
    
    for (auto& socket : sockets) {
        socket->Close ();
        EXPECT_FALSE (socket->IsConnected ());
    }
    
    OS::Timing::Sleep (2000u); // Make sure cleaner ticked once
    EXPECT_EQ (0u, server.GetClientCount ());
    
    server.Stop ();
    
    EXPECT_EQ (0u, server.GetClientCount ());
}

TEST_F (TcpServerTester, ClosingServer) {
    
    auto factory (std::make_unique<TestClientFactory>());
    TCP::Server server (IP_FOR_TESTING, PORT_FOR_TESTING, std::move (factory));
    
    server.Start();
    
    OS::Timing::Sleep (100u);
    EXPECT_EQ (0u, server.GetClientCount ());
    
    std::vector<std::unique_ptr<OS::Socket>> sockets;
    for (std::size_t i (0u); i < 5u; ++i) {
        sockets.emplace_back (std::make_unique<OS::Socket> (IP_FOR_TESTING, PORT_FOR_TESTING));
        sockets.back()->Initialize();
        EXPECT_TRUE (sockets.back()->Connect ());
        EXPECT_TRUE (sockets.back()->IsConnected ());
    }
    
    OS::Timing::Sleep (100u);
    EXPECT_EQ (sockets.size (), server.GetClientCount ());
    
    server.Stop ();
    OS::Timing::Sleep (100u);
    
    for (auto& socket : sockets) {
        std::vector<uint8_t> buffer (1u);
        EXPECT_LE (socket->Receive (buffer), 0); // sockets must receive before they see anything happening
        EXPECT_FALSE (socket->IsConnected ());
    }
    
    EXPECT_EQ (0u, server.GetClientCount ());
}