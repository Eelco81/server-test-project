
#include <algorithm>

#include "Mutex.h"
#include "Log.h"
#include "Socket.h"
#include "Thread.h"
#include "TriggerThread.h"

#include "TcpServer.h"
#include "TcpClient.h"
#include "TcpClientFactory.h"

namespace {

class ListenThread : public OS::Thread {
    
public:
    ListenThread (TCP::Server& inServer, const std::string& inAddress, const std::string& inPort) :
        Thread ("ServerListener"),
        mServer (inServer),
        mSocket (inAddress, inPort)
    {
        mSocket.Initialize ();
    }
    ~ListenThread () = default;

    virtual void Execute () override {
        mSocket.Listen (); 
        LOGINFO << "TCP Server listening at " << mSocket.GetAddress () << ":" << mSocket.GetPortNumber ();
        while (mSocket.IsListening ()) {
            auto clientSocket = std::make_unique <OS::Socket> (mSocket.GetAddress (), mSocket.GetPortNumber ());
            if (mSocket.Accept (*clientSocket)) { // blocking call
                mServer.RegisterClient (std::move (clientSocket));
            }
        }
        LOGINFO << "TCP Server stopped at " << mSocket.GetAddress () << ":" << mSocket.GetPortNumber ();
    }

    virtual void Kill () override {
        mSocket.Close();
    }
    
private:
    TCP::Server& mServer;
    OS::Socket mSocket;
};

} // end anonymous namespace

TCP::Server::Server (const std::string& inAddress, const std::string& inPort, std::shared_ptr<ClientFactory> inFactory) :
    mMutex (std::make_unique <OS::Mutex> ()),
    mListener (std::make_unique <ListenThread> (*this, inAddress, inPort)),
    mCleaner (std::make_unique <APP::TriggerThread<Server>> ("CleanupServer", 1000u, this, &Server::CleanUp)),
    mFactory (inFactory)
{
}

TCP::Server::~Server () {
    Stop ();
}

void TCP::Server::Start () {
    mListener->Spawn ();
    mCleaner->Spawn ();
}

void TCP::Server::Stop () {
    
    mListener->Kill ();
    mListener->Join ();
    
    {
        OS::SingleLock lock (*mMutex);
        std::for_each (mClients.begin (), mClients.end (), [] (ClientPtr& client) { client->Stop (); });
    }
    
    CleanUp ();
    
    mCleaner->Kill ();
    mCleaner->Join ();
}

std::size_t TCP::Server::GetClientCount () const {
    return mClients.size ();
}

// called from listener thread
void TCP::Server::RegisterClient (std::unique_ptr <OS::Socket> inClientSocket) {
    
    LOGDEBUG << "[TcpServer] Registering connected client with id "  << inClientSocket->GetId ();
    OS::SingleLock lock (*mMutex);
    mClients.emplace_back (mFactory->Create (std::move (inClientSocket)));
    mClients.back ()->Start (); // No need to listen to the return value of start
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
