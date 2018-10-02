
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
    ReceiveThread (OS::MessageStream<std::vector<uint8_t>,std::vector<uint8_t>>& inStream, std::shared_ptr<OS::Socket> inSocket) :
        Thread (std::string ("Client-") + std::to_string (inSocket->GetId ())),
        mStream (inStream),
        mSocket (inSocket)
    {
    }

    virtual void Execute () override {
        
        while (mSocket->IsConnected ()) {
            
            std::vector<uint8_t> buffer (MAX_BUFFER_SIZE);
            const auto result (mSocket->Receive (buffer)); // blocking call
            if (result > 0) {
                buffer.resize (result);
                mStream.Write (buffer);
            }
        }
        
        mSocket->Close ();
    }

    virtual void Kill () override {
        mSocket->Close();
    }
    
private:
    OS::MessageStream<std::vector<uint8_t>, std::vector<uint8_t>>& mStream;
    std::shared_ptr<OS::Socket> mSocket;
};

} // end anonymous namespace

void TCP::Client::Stream::Write (const Packet& inPacket) {
    Done (inPacket);
};

TCP::Client::Client (std::string inAddress, std::string inPort) :
    mSocket (std::make_shared<OS::Socket> (inAddress, inPort))
{
    mSocket->Initialize ();
    mThread = std::make_unique<ReceiveThread> (GetReadStream (), mSocket);
    GetWriteStream ().Pipe (this, &Client::Send);
    //GetWriteStream ().Pipe (mSocket.get (), &OS::Socket::Send);
}

TCP::Client::Client (std::unique_ptr <OS::Socket> inSocket) :
    mSocket (std::move (inSocket)),
    mThread (std::make_unique<ReceiveThread> (GetReadStream (), mSocket))
{
    GetWriteStream ().Pipe (this, &Client::Send);
    //GetWriteStream ().Pipe (mSocket.get (), &OS::Socket::Send);
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

void TCP::Client::Quit () {
    mThread->Kill ();
}

bool TCP::Client::IsConnected () const {
    return mSocket->IsConnected ();
}

int TCP::Client::GetId () const {
    return mSocket->GetId ();
}

void TCP::Client::Send (const std::vector<uint8_t>& inBuffer) {
    mSocket->Send (inBuffer);
}
