
#define MAX_BUFFER_SIZE 2048

#include <string>
#include <iostream>
#include "Thread.h"
#include "Network.h"
#include "Socket.h"

#include "TcpServer.h"
#include "TcpClient.h"

namespace {

    class ReceiveThread : public OS::Thread {
        
    public:
        ReceiveThread (TCP::Client& inClient, std::shared_ptr<OS::Socket> inSocket) :
            Thread (std::string ("Client-") + std::to_string (inSocket->GetId ())),
            mClient (inClient),
            mSocket (inSocket)
        {
        }

        virtual void Execute () override {
            
            while (mSocket->IsConnected ()) {
                
                std::vector<uint8_t> buffer (MAX_BUFFER_SIZE);
                const auto result (mSocket->Receive (buffer)); // blocking call
                if (result > 0) {
                    buffer.resize (result);
                    mClient.OnReceived (buffer);
                }
            }
            
            mSocket->Close ();
        }

        virtual void Kill () override {
            mSocket->Close();
        }
        
    private:
        TCP::Client& mClient;
        std::shared_ptr<OS::Socket> mSocket;
    };
}

TCP::Client::Client (std::string inAddress, std::string inPort) :
    mSocket (std::make_shared<OS::Socket> (inAddress, inPort))
{
    mSocket->Initialize ();
    mThread = std::make_unique<ReceiveThread> (*this, mSocket);
}

TCP::Client::Client (std::unique_ptr <OS::Socket> inSocket) :
    mSocket (std::move (inSocket)),
    mThread (std::make_unique<ReceiveThread> (*this, mSocket))
{
}

TCP::Client::~Client () {
    Stop ();
}

bool TCP::Client::Start () {
    if (!mSocket->IsConnected () && !mSocket->Connect ()) {
        return false;
    }
    mThread->Spawn ();
    return true;
}

void TCP::Client::Stop () {
    mThread->Kill ();
    mThread->Join ();
}

bool TCP::Client::IsConnected () const {
    return mSocket->IsConnected ();
}

int TCP::Client::GetId () const {
    return mSocket->GetId ();
}

bool TCP::Client::Send (const std::vector<uint8_t>& inBuffer) {
    return mSocket->Send (inBuffer) > 0;
}

bool TCP::Client::Send (const uint8_t* inData, std::size_t inSize) {
    return mSocket->Send (inData, inSize) > 0;
}