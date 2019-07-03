
#include <algorithm>

#include "Mutex.h"
#include "Log.h"
#include "TriggerThread.h"
#include "HoldingThread.h"

#include "TcpServer.h"
#include "TcpClient.h"
#include "TcpClientFactory.h"

TCP::Server::Server (const std::string& inAddress, const std::string& inPort, std::shared_ptr<ClientFactory> inFactory) :
    mMutex (std::make_unique <OS::Mutex> ()),
    mListener (std::make_unique <APP::HoldingThread<Server>> ("ServerListener", this, &TCP::Server::WaitForConnections)),
    mCleaner (std::make_unique <APP::TriggerThread<Server>> ("ServerCleaner", 1000000u, this, &TCP::Server::CleanUp)),
    mFactory (inFactory),
    mSocket (inAddress, inPort)
{
    mSocket.Initialize ();
}

TCP::Server::~Server () {
    Stop ();
}

void TCP::Server::Start () {
    mListener->Spawn ();
    mCleaner->Spawn ();
}

void TCP::Server::Stop () {
    
    mSocket.Close ();
    mListener->Join ();
    
    {
        OS::SingleLock lock (*mMutex);
        std::for_each (mClients.begin (), mClients.end (), [] (ClientPtr& client) { client->Stop (); });
    }
    
    CleanUp ();
    
    mCleaner->Kill ();
    mCleaner->Join ();
}

void TCP::Server::Broadcast (const TCP::Packet& inPacket) {
    
    OS::SingleLock lock (*mMutex);
    
    for (auto& client : mClients) {
        client->Send (inPacket);
    }
}

void TCP::Server::Broadcast (const TCP::RawPacket& inPacket) {
    
    OS::SingleLock lock (*mMutex);
    
    for (auto& client : mClients) {
        client->Send (inPacket);
    }
}

std::size_t TCP::Server::GetClientCount () const {
    return mClients.size ();
}

// called from listener thread
void TCP::Server::WaitForConnections () {
    
    LOGINFO << "TCP Server listening at " << mSocket.GetAddress () << ":" << mSocket.GetPortNumber ();

    mSocket.Listen ();
    while (mSocket.IsListening ()) {
        
        auto clientSocket = std::make_unique <OS::Socket> (mSocket.GetAddress (), mSocket.GetPortNumber ());
        if (mSocket.Accept (*clientSocket)) { // blocking call
        
            LOGDEBUG << "[TcpServer] Registering connected client with id "  << clientSocket->GetId ();
            
            OS::SingleLock lock (*mMutex);
            mClients.emplace_back (mFactory->Create (std::move (clientSocket)));
            mClients.back ()->Start (); // No need to listen to the return value of start
        }
    }
    
    LOGINFO << "TCP Server stopped at " << mSocket.GetAddress () << ":" << mSocket.GetPortNumber ();
}

// called from cleaner thread
bool TCP::Server::CleanUp () {

    auto remover = [] (const ClientPtr& client) {
        if (!client->IsConnected ()) {
            client->Stop ();
            LOGDEBUG << "[TcpServer] Unregistering connected client with id " << client->GetId ();
            return true;
        }
        return false;
    };

    OS::SingleLock lock (*mMutex);
    mClients.erase (std::remove_if (mClients.begin (), mClients.end (), remover), mClients.end ());

    return true;
}