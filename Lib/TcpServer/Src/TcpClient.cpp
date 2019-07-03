
#define MAX_BUFFER_SIZE 2048

#include <string>
#include <iostream>
#include "HoldingThread.h"
#include "Network.h"
#include "Socket.h"

#include "TcpServer.h"
#include "TcpClient.h"

TCP::Client::Client (std::string inAddress, std::string inPort) :
    TCP::Client::Client (std::make_unique<OS::Socket> (inAddress, inPort))
{
    mSocket->Initialize ();
}

TCP::Client::Client (std::unique_ptr <OS::Socket> inSocket) :
    mThread (std::make_unique<APP::HoldingThread<Client>> ("TcpClient-" + std::to_string (inSocket->GetId ()), this, &TCP::Client::WaitForData)),
    mSocket (std::move (inSocket))
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
    Quit ();
    mThread->Join ();
}

void TCP::Client::Quit () {
    // This will exit holding thread WaitForData (), since it will cause
    // the blocking Socket::Receive () to exit with a failure.
    mSocket->Close ();
}

bool TCP::Client::IsConnected () const {
    return mSocket->IsConnected ();
}

int TCP::Client::GetId () const {
    return mSocket->GetId ();
}

void TCP::Client::Send (const TCP::Packet& inBuffer) {
    mSocket->Send (inBuffer);
}

void TCP::Client::Send (const TCP::RawPacket& inPacket) {
    mSocket->Send (std::get<0> (inPacket), std::get<1> (inPacket));
}

void TCP::Client::WaitForData () {
    
    Packet buffer (MAX_BUFFER_SIZE);

    // handle data while the socket is alive
    while (mSocket->IsConnected ()) {
        
        // blocking call
        const auto result (mSocket->Receive (buffer.data (), buffer.size ())); 

        // if there is data, emit it,
        if (result > 0) {
            sDataAvailable.Emit (RawPacket (buffer.data (), static_cast<std::size_t> (result)));
        }
    }

    // the connection is closed
    sClosed.Emit ();
}
