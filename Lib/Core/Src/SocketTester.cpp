
#include "gtest/gtest.h"

#include "Log.h"
#include "Network.h"
#include "Buffer.h"
#include "Thread.h"
#include "Socket.h"

#define IP_FOR_TESTING "127.0.0.1"
#define PORT_FOR_TESTING "1701"

namespace {

    class SocketThread : public OS::Thread {
    public:
        SocketThread (const std::string& inName) :
            Thread (inName),
            mSocket (IP_FOR_TESTING, PORT_FOR_TESTING),
            mInBuffer (100u),
            mOutBuffer (100u)
        {
        }
        virtual ~SocketThread () {
            mSocket.Close ();
        }
    public:
        void Initialize () { mSocket.Initialize (); }
        virtual void Close () { mSocket.Close (); }
        bool IsListening () const { return mSocket.IsListening (); }
        bool IsConnected () const { return mSocket.IsConnected (); }
    protected:
        OS::Socket mSocket;
    public:
        OS::Buffer mInBuffer;
        OS::Buffer mOutBuffer;
    };

    class ServerThread : public SocketThread {
    public:
        ServerThread () : 
            SocketThread ("ServerThread"),
            mClientSocket (IP_FOR_TESTING, PORT_FOR_TESTING)
        {
            mOutBuffer.SetData ("FROM_SERVER", 11u);
        }
        virtual ~ServerThread () {}
        virtual void Close () override {
            mClientSocket.Close ();
            SocketThread::Close ();
        }
    protected:
        OS::Socket mClientSocket;
    };

    class ClientThread : public SocketThread {
    public:
        ClientThread () : 
            SocketThread ("ClientThread") 
        {
            mOutBuffer.SetData ("FROM_CLIENT", 11u);
        }
        virtual ~ClientThread () {}
    };

}

class SocketTester : public ::testing::Test {
    void SetUp () {
        OS::Log::Instance ().Initialize (OS::Log::kTrace);
        OS::Network::Initialize ();
    }
    void TearDown () {
        OS::Network::Done ();
        OS::Log::Instance ().Flush ();
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
            EXPECT_TRUE (mClientSocket.Send (mOutBuffer));
            EXPECT_TRUE (mClientSocket.Receive (mInBuffer));
            EXPECT_TRUE (mSocket.IsListening ());
            EXPECT_TRUE (mClientSocket.IsConnected ());
        }
    };
    
    class Client : public ClientThread {
        virtual void Execute () override {
            EXPECT_FALSE (mSocket.IsConnected ());
            EXPECT_TRUE (mSocket.Connect ());
            EXPECT_TRUE (mSocket.IsConnected ());
            EXPECT_TRUE (mSocket.Send (mOutBuffer));
            EXPECT_TRUE (mSocket.Receive (mInBuffer));
            EXPECT_TRUE (mSocket.IsConnected ());
        }
    };

    Server server;
    Client client;
    
    std::string output;

    client.mInBuffer.ToString (output);
    ASSERT_EQ (std::string (""), std::string (output));

    server.mInBuffer.ToString (output);
    ASSERT_EQ (std::string (""), std::string (output));

    server.Initialize ();
    server.Spawn ();

    while (!server.IsListening ()) {}

    client.Initialize ();
    client.Spawn ();

    client.Join ();
    client.Close ();

    server.Join ();
    server.Close ();

    client.mInBuffer.ToString (output);
    ASSERT_EQ (std::string ("FROM_SERVER"), std::string (output));

    server.mInBuffer.ToString (output);
    ASSERT_EQ (std::string ("FROM_CLIENT"), std::string (output));
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
            EXPECT_FALSE (mClientSocket.Receive (mInBuffer)); // received 0
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

    client.Initialize ();
    client.Spawn ();

    client.Join ();
    client.Close ();

    server.Join ();
    server.Close ();
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
            EXPECT_FALSE (mSocket.Receive (mInBuffer)); // received 0
            EXPECT_FALSE (mSocket.IsConnected ());
        }
    };

    Server server;
    Client client;

    server.Initialize ();
    server.Spawn ();

    while (!server.IsListening ()) {}

    client.Initialize ();
    client.Spawn ();

    client.Join ();
    client.Close ();

    server.Join ();
    server.Close ();
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
            EXPECT_FALSE (mSocket.Receive (mInBuffer)); // received 0
            EXPECT_FALSE (mSocket.IsConnected ());
        }
    };

    Server server;
    Client client;

    server.Initialize ();
    server.Spawn ();

    while (!server.IsListening ()) {}

    client.Initialize ();
    client.Spawn ();

    client.Join ();
    client.Close ();

    server.Join ();
    server.Close ();
}

