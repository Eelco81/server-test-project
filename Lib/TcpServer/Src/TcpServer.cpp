
#include <algorithm>

#include "Mutex.h"
#include "Log.h"
#include "Socket.h"
#include "Thread.h"
#include "Task.h"
#include "PeriodicThread.h"

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
        ~ListenThread () {}

        virtual void Execute () override {
            mSocket.Listen (); 
            LOGMESSAGE (OS::Log::kInfo, "TCP Server listening at " + mSocket.GetAddress () + ":" + mSocket.GetPortNumber ());
            while (mSocket.IsListening ()) {
                auto clientSocket = std::make_unique <OS::Socket> (mSocket.GetAddress (), mSocket.GetPortNumber ());
                if (mSocket.Accept (*clientSocket)) { // blocking call
                    mServer.RegisterClient (std::move (clientSocket));
                }
            }
            LOGMESSAGE (OS::Log::kInfo, "TCP Server stopped at " + mSocket.GetAddress () + ":" + mSocket.GetPortNumber ());
        }

        virtual void Kill () override {
            mSocket.Close();
        }
        
    private:
        TCP::Server& mServer;
        OS::Socket mSocket;
    };

    class CleanUpTask : public APP::Task {
    public:
        CleanUpTask (TCP::Server& inServer) : 
            Task ("CleanupServerTask"),
            mServer (inServer) 
        {
        }
        virtual bool Step () override {
            mServer.CleanUp ();
            return true;
        }
    private:
        TCP::Server& mServer;
    };

    class CleanupThread : public APP::PeriodicThread {
    public:
        CleanupThread (TCP::Server& inServer) :
            PeriodicThread ("CleanupServer", 1000u)
        {
            AddTask (std::make_unique <CleanUpTask> (inServer));
        }
        ~CleanupThread () {}
    };

} // end anonymous namespace

TCP::Server::Server (const std::string& inAddress, const std::string& inPort, std::shared_ptr<ClientFactory> inFactory) :
    mMutex (std::make_unique <OS::Mutex> ()),
    mListener (std::make_unique <ListenThread> (*this, inAddress, inPort)),
    mCleaner (std::make_unique <CleanupThread> (*this)),
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
    
    mMutex->Lock ();
    std::for_each (mClients.begin (), mClients.end (), [] (ClientPtr& client) { client->Stop (); });
    mMutex->UnLock ();
    
    CleanUp ();
    
    mCleaner->Kill ();
    mCleaner->Join ();
}

std::size_t TCP::Server::GetClientCount () const {
    return mClients.size ();
}

// called from listener thread
void TCP::Server::RegisterClient (std::unique_ptr <OS::Socket> inClientSocket) {
    LOGMESSAGE (OS::Log::kDebug, std::string ("[TcpServer] Registering connected client with id ") + std::to_string (inClientSocket->GetId ()));
    mMutex->Lock ();
    mClients.emplace_back (mFactory->Create (std::move (inClientSocket)));
    mClients.back ()->Start (); // No need to listen to the return value of start
    mMutex->UnLock ();
}

// called from cleaner thread
void TCP::Server::CleanUp () {

    auto remover = [] (const ClientPtr& client) {
        if (!client->IsConnected ()) {
            client->Stop ();
            LOGMESSAGE (OS::Log::kDebug, std::string ("[TcpServer] Unregistering connected client with id ") + std::to_string (client->GetId ()));
            return true;
        }
        return false;
    };

    mMutex->Lock ();
    mClients.erase (std::remove_if (mClients.begin (), mClients.end (), remover), mClients.end ());
    mMutex->UnLock ();
}
