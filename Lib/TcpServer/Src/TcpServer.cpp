
#include <algorithm>

#include "Log.h"

#include "Buffer.h"

#include "Socket.h"
#include "Thread.h"
#include "Task.h"
#include "PeriodicThread.h"

#include "TcpServer.h"
#include "TcpClient.h"

#include <iostream>

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
            LOGMESSAGE (OS::Log::kInfo, "[TcpServer] Listening at " + mSocket.GetAddress () + ":" + mSocket.GetPortNumber ());
            while (mSocket.IsListening ()) {
                auto clientSocket = std::make_unique <OS::Socket> ("", "");
                if (mSocket.Accept (*clientSocket)) { // blocking call
                    mServer.RegisterClient (std::move (clientSocket));
                }
            }
            LOGMESSAGE (OS::Log::kInfo, "[TcpServer] Stopped at " + mSocket.GetAddress () + ":" + mSocket.GetPortNumber ());
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

}

TCP::Server::Server (const std::string& inAddress, const std::string& inPort) :
    mListener (std::make_unique <ListenThread> (*this, inAddress, inPort)),
    mCleaner (std::make_unique <CleanupThread> (*this))
{
}

TCP::Server::~Server () = default;

void TCP::Server::Start () {
    mListener->Spawn ();
    mCleaner->Spawn ();
}

void TCP::Server::Stop() {
    
    mListener->Kill();
    mListener->Join();
    
    mMutex.lock ();
    std::for_each (mClients.begin (), mClients.end (), [] (ClientPtr& client) { client->Kill(); });
    mMutex.unlock ();
    
    CleanUp();
    
    mCleaner->Kill();
    mCleaner->Join();
}

void TCP::Server::BroadCast (const OS::Buffer& inBuffer) {
    mMutex.lock ();
    std::for_each (mClients.begin (), mClients.end (), [&inBuffer] (ClientPtr& client) { client->Send (inBuffer); });
    mMutex.unlock ();
}

void TCP::Server::Send (unsigned inClientId, const OS::Buffer& inBuffer) {
    mMutex.lock ();
    auto clientIterator = std::find_if (mClients.begin (), mClients.end (), [inClientId] (const ClientPtr& client) { return client->GetId () == inClientId; });
    if (clientIterator != mClients.end ()) {
        (*clientIterator)->Send (inBuffer);
    }
    mMutex.unlock ();
}

std::size_t TCP::Server::GetClientCount() const {
    return mClients.size();
}

// called from listener thread
void TCP::Server::RegisterClient (std::unique_ptr <OS::Socket> inClientSocket) {
    LOGMESSAGE (OS::Log::kInfo, std::string ("[TcpServer] Registering connected client with id ") + std::to_string (inClientSocket->GetId ()));
    mMutex.lock ();
    mClients.emplace_back (std::make_unique <Client> (std::move (inClientSocket)));
    mClients.back ()->Spawn ();
    mMutex.unlock ();
}

// called from cleaner thread
void TCP::Server::CleanUp () {

    auto remover = [] (const ClientPtr& client) {
        if (client->GetStatus () == OS::Thread::kDone) {
            client->Join ();
            LOGMESSAGE (OS::Log::kInfo, std::string ("[TcpServer] Unregistering connected client with id ") + std::to_string (client->GetId ()));
            return true;
        }
        return false;
    };

    mMutex.lock ();
    mClients.erase (std::remove_if (mClients.begin (), mClients.end (), remover), mClients.end ());
    mMutex.unlock ();
}
