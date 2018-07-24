
#include "gtest/gtest.h"

#include "Timing.h"
#include "Log.h"
#include "Network.h"
#include "Buffer.h"
#include "Thread.h"
#include "Socket.h"

#define IP_FOR_TESTING "127.0.0.1"
#define PORT_FOR_TESTING "1701"

namespace {

    const int kBufferSize (1000);
    
    class SocketThread : public OS::Thread {
    public:
        SocketThread (const std::string& inName, uint8_t inBufferValue) :
            Thread (inName),
            mSocket (IP_FOR_TESTING, PORT_FOR_TESTING),
            mInBuffer (kBufferSize + 1u, 0x00),// for simplicity make it bugger than outBuffer
            mOutBuffer (kBufferSize, inBufferValue)
        {
        }
        virtual ~SocketThread () {
            mSocket.Close ();
        }
    public:
        void Initialize () { mSocket.Initialize (); }
        virtual void Kill () override { mSocket.Close (); }
        bool IsListening () const { return mSocket.IsListening (); }
        bool IsConnected () const { return mSocket.IsConnected (); }
    protected:
        OS::Socket mSocket;
    public:
        std::vector<uint8_t> mInBuffer;
        const std::vector<uint8_t> mOutBuffer;
    };

    class ServerThread : public SocketThread {
    public:
        ServerThread () : 
            SocketThread ("ServerThread", 0xFF),
            mClientSocket (IP_FOR_TESTING, PORT_FOR_TESTING)
        {
        }
        virtual ~ServerThread () {}
        virtual void Kill () override {
            mClientSocket.Close ();
            SocketThread::Kill ();
        }
    protected:
        OS::Socket mClientSocket;
    };

    class ClientThread : public SocketThread {
    public:
        ClientThread () : 
            SocketThread ("ClientThread", 0x0F) 
        {
        }
        virtual ~ClientThread () {}
    };

} // end anonymous namespace

class SocketTester : public ::testing::Test {
    void SetUp () {
        OS::Log::Instance ().Initialize (OS::Log::kTrace);
        OS::Network::Initialize ();
    }
    void TearDown () {
        OS::Network::Done ();
        OS::Log::Instance ().Flush ();
        OS::Timing::Sleep (100u); // give the OS some time to clean up the socket
    }
};

TEST_F (SocketTester, BasicDataTransfer) {

    class Server : public ServerThread {
        virtual void Execute () override {
            EXPECT_FALSE (mSocket.IsListening ());
            EXPECT_FALSE (mClientSocket.IsConnected ());
            EXPECT_TRUE (mSocket.Listen ());
            EXPECT_TRUE (mSocket.IsListening ());
            EXPECT_TRUE (mSocket.Accept (mClientSocket));
            EXPECT_TRUE (mClientSocket.IsConnected ());
            EXPECT_EQ (mClientSocket.Send (mOutBuffer), kBufferSize);
            EXPECT_EQ (mClientSocket.Receive (mInBuffer), kBufferSize);
            EXPECT_TRUE (mSocket.IsListening ());
            EXPECT_TRUE (mClientSocket.IsConnected ());
        }
    };
    
    class Client : public ClientThread {
        virtual void Execute () override {
            EXPECT_FALSE (mSocket.IsConnected ());
            EXPECT_TRUE (mSocket.Connect ());
            EXPECT_TRUE (mSocket.IsConnected ());
            EXPECT_EQ (mSocket.Send (mOutBuffer), kBufferSize);
            EXPECT_EQ (mSocket.Receive (mInBuffer), kBufferSize);
            EXPECT_TRUE (mSocket.IsConnected ());
        }
    };

    Server server;
    Client client;

    server.Initialize ();
    server.Spawn ();

    while (!server.IsListening ()) {}
    OS::Timing::Sleep(1000u);
    
    client.Initialize ();
    client.Spawn ();

    client.Join ();
    server.Join ();
    
    ASSERT_TRUE (std::equal (server.mOutBuffer.begin(), server.mOutBuffer.end(), client.mInBuffer.begin()));
    ASSERT_TRUE (std::equal (client.mOutBuffer.begin(), client.mOutBuffer.end(), server.mInBuffer.begin()));
}

TEST_F (SocketTester, ClosingClients) {

    class Server : public ServerThread {
        void Execute () override {
            EXPECT_FALSE (mSocket.IsListening ());
            EXPECT_FALSE (mClientSocket.IsConnected ());
            EXPECT_TRUE (mSocket.Listen ());
            EXPECT_TRUE (mSocket.IsListening ());
            EXPECT_TRUE (mSocket.Accept (mClientSocket));
            EXPECT_TRUE (mClientSocket.IsConnected ());
            EXPECT_LE (mClientSocket.Receive (mInBuffer), 0); // received 0
            EXPECT_FALSE (mClientSocket.IsConnected ());
            EXPECT_TRUE (mSocket.IsListening ());
        }
    };

    class Client : public ClientThread {
        virtual void Execute () override {
            EXPECT_FALSE (mSocket.IsConnected ());
            EXPECT_TRUE (mSocket.Connect ());
            EXPECT_TRUE (mSocket.IsConnected ());
            mSocket.Close ();
            EXPECT_FALSE (mSocket.IsConnected ());
        }
    };

    Server server;
    Client client;

    server.Initialize ();
    server.Spawn ();

    while (!server.IsListening ()) {}
    OS::Timing::Sleep(1000u);
    
    client.Initialize ();
    client.Spawn ();

    client.Join ();
    server.Join ();
}

TEST_F (SocketTester, ServerClosesConnection) {

    class Server : public ServerThread {
        void Execute () override {
            EXPECT_FALSE (mSocket.IsListening ());
            EXPECT_FALSE (mClientSocket.IsConnected ());
            EXPECT_TRUE (mSocket.Listen ());
            EXPECT_TRUE (mSocket.IsListening ());
            EXPECT_TRUE (mSocket.Accept (mClientSocket));
            EXPECT_TRUE (mClientSocket.IsConnected ());
            mClientSocket.Close ();
            EXPECT_FALSE (mClientSocket.IsConnected ());
            EXPECT_TRUE (mSocket.IsListening ());
        }
    };

    class Client : public ClientThread {
        virtual void Execute () override {
            EXPECT_FALSE (mSocket.IsConnected ());
            EXPECT_TRUE (mSocket.Connect ());
            EXPECT_TRUE (mSocket.IsConnected ());
            EXPECT_LE (mSocket.Receive (mInBuffer), 0); // received 0
            EXPECT_FALSE (mSocket.IsConnected ());
        }
    };

    Server server;
    Client client;

    server.Initialize ();
    server.Spawn ();

    while (!server.IsListening ()) {}
    OS::Timing::Sleep(1000u);
    
    client.Initialize ();
    client.Spawn ();

    client.Join ();
    server.Join ();
}

TEST_F (SocketTester, ServerCloses) {

    class Server : public ServerThread {
        void Execute () override {
            EXPECT_FALSE (mSocket.IsListening ());
            EXPECT_FALSE (mClientSocket.IsConnected ());
            EXPECT_TRUE (mSocket.Listen ());
            EXPECT_TRUE (mSocket.IsListening ());
            EXPECT_TRUE (mSocket.Accept (mClientSocket));
            EXPECT_TRUE (mClientSocket.IsConnected ());
            mSocket.Close ();
            EXPECT_FALSE (mSocket.IsListening ());
            // Note: the client socket need to be closed separately
            mClientSocket.Close ();
            EXPECT_FALSE (mClientSocket.IsConnected ());
        }
    };

    class Client : public ClientThread {
        virtual void Execute () override {
            EXPECT_FALSE (mSocket.IsConnected ());
            EXPECT_TRUE (mSocket.Connect ());
            EXPECT_TRUE (mSocket.IsConnected ());
            EXPECT_LE (mSocket.Receive (mInBuffer), 0); // received 0
            EXPECT_FALSE (mSocket.IsConnected ());
        }
    };

    Server server;
    Client client;

    server.Initialize ();
    server.Spawn ();

    while (!server.IsListening ()) {}
    OS::Timing::Sleep(1000u);
    
    client.Initialize ();
    client.Spawn ();

    client.Join ();
    server.Join ();
}
