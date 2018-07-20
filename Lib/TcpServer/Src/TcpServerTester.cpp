
#include "gtest/gtest.h"

#include "Timing.h"
#include "Log.h"
#include "Network.h"
#include "Socket.h"
#include "TcpServer.h"

#define IP_FOR_TESTING "127.0.0.1"
#define PORT_FOR_TESTING "1703"

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

TEST_F (TcpServerTester, TcpServerTester) {
    
    
    TCP::Server server (IP_FOR_TESTING, PORT_FOR_TESTING);
    
    server.Start();
    
    OS::Socket client(IP_FOR_TESTING, PORT_FOR_TESTING);
    client.Initialize();
    
    EXPECT_EQ (0u, server.GetClientCount ());
    
    EXPECT_TRUE (client.Connect());
    OS::Timing::Sleep (100);
    
    EXPECT_EQ (1u, server.GetClientCount ());
    
    client.Close();
    OS::Timing::Sleep (100);

    server.Stop();
    
    EXPECT_EQ (0u, server.GetClientCount ());

}