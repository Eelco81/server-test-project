
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
        GetReadStream ().Pipe (GetWriteStream ());
    }
};

class HarvestClient : public TCP::Client {
public:
    HarvestClient (std::string inAddress, std::string inPort) :
       TCP::Client (inAddress, inPort) 
    {
        GetReadStream ().Pipe (this, &HarvestClient::HandlePacket);
    }
    void HandlePacket (const std::vector<uint8_t>& inBuffer) {
        std::copy (inBuffer.begin (), inBuffer.end (), std::back_inserter (mData));
    }
    std::vector<uint8_t> mData = {};
};

class TestClientFactory : public TCP::ClientFactory {
public:
    std::unique_ptr<TCP::Client> Create (std::unique_ptr<OS::Socket> inSocket) const override {
        return std::make_unique<EchoClient> (std::move (inSocket));
    }
};

} // end anonymous namespace

class TcpServerTester : public ::testing::Test {
    void TearDown () {
        OS::Log::Instance ().Flush ();
    }
};

TEST_F (TcpServerTester, DataTransfer) {
    
    auto factory (std::make_shared<TestClientFactory>());
    TCP::Server server (IP_FOR_TESTING, PORT_FOR_TESTING, std::move (factory));
    
    server.Start();
    OS::Timing::Sleep (100000u);

    HarvestClient client (IP_FOR_TESTING, PORT_FOR_TESTING);
    EXPECT_TRUE (client.Start ());
    
    OS::Timing::Sleep (100000u);
    
    const std::vector<uint8_t> buffer (1000, 0xFF);
    client.Send (buffer);
    
    OS::Timing::Sleep (100000u);
    
    EXPECT_EQ (buffer.size (), client.mData.size ());
    EXPECT_TRUE (std::equal (buffer.begin (), buffer.end (), client.mData.begin ()));

}

TEST_F (TcpServerTester, MultipleConnnections) {
    
    auto factory (std::make_shared<TestClientFactory>());
    TCP::Server server (IP_FOR_TESTING, PORT_FOR_TESTING, std::move (factory));
    
    server.Start();
    
    OS::Timing::Sleep (100000u);
    EXPECT_EQ (0u, server.GetClientCount ());
    
    std::vector<std::unique_ptr<HarvestClient>> clients;
    for (std::size_t i (0u); i < 5u; ++i) {
        clients.emplace_back (std::make_unique<HarvestClient> (IP_FOR_TESTING, PORT_FOR_TESTING));
        EXPECT_TRUE (clients.back ()->Start ());
    }
    
    OS::Timing::Sleep (100000u);
    EXPECT_EQ (clients.size (), server.GetClientCount ());
    
    for (auto& client : clients) {
        client->Stop ();
        EXPECT_FALSE (client->IsConnected ());
    }
    
    OS::Timing::Sleep (2000000u); // Make sure cleaner ticked once
    EXPECT_EQ (0u, server.GetClientCount ());
}

TEST_F (TcpServerTester, ClosingServer) {
    
    auto factory (std::make_shared<TestClientFactory>());
    TCP::Server server (IP_FOR_TESTING, PORT_FOR_TESTING, std::move (factory));
    
    server.Start ();
    
    OS::Timing::Sleep (100000u);
    EXPECT_EQ (0u, server.GetClientCount ());
    
    std::vector<std::unique_ptr<HarvestClient>> clients;
    for (std::size_t i (0u); i < 5u; ++i) {
        clients.emplace_back (std::make_unique<HarvestClient> (IP_FOR_TESTING, PORT_FOR_TESTING));
        EXPECT_TRUE (clients.back ()->Start ());
    }
    
    OS::Timing::Sleep (100000u);
    EXPECT_EQ (clients.size (), server.GetClientCount ());
    
    server.Stop ();
    OS::Timing::Sleep (100000u);
    
    for (auto& client : clients) {
        EXPECT_FALSE (client->IsConnected ());
    }
    
    EXPECT_EQ (0u, server.GetClientCount ());
}