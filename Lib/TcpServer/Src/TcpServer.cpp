
#include <algorithm>

#include "Buffer.h"

#include "Socket.h"
#include "Thread.h"
#include "Task.h"
#include "PeriodicThread.h"

#include "TcpServer.h"
#include "TcpClient.h"

#include "Router.h"

namespace {

    class ListenThread : public Core::Thread {
        
    public:
        ListenThread (TCP::Server& inServer, const std::string& inAddress, const std::string& inPort) :
            Thread ("ServerListener"),
            mServer (inServer),
            mSocket (inAddress, inPort)
        {
            mSocket.Initialize ();
        }
        ~ListenThread () {}

        void Execute () override {
            mSocket.Listen ();
            while (mSocket.IsListening ()) {
                auto clientSocket = std::make_unique <Core::Socket> ("", "");
                if (mSocket.Accept (*clientSocket)) {
                    mServer.RegisterClient (std::move (clientSocket));
                }
            }
        }

    private:
        TCP::Server& mServer;
        Core::Socket mSocket;
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
    mCleaner (std::make_unique <CleanupThread> (*this)),
    mRouter (std::make_shared <Router> ())
{
}

TCP::Server::~Server () {
}

void TCP::Server::Start () {
    mListener->Spawn ();
    mCleaner->Spawn ();
}

void TCP::Server::BroadCast (const Core::Buffer& inBuffer) {
    mMutex.lock ();
    std::for_each (mClients.begin (), mClients.end (), [&](std::unique_ptr<Client>& client) {
        client->Send (inBuffer);
    });
    mMutex.unlock ();
};

void TCP::Server::Send (unsigned inClientId, const Core::Buffer& inBuffer) {
    mMutex.lock ();
    auto clientIterator = std::find_if (mClients.begin (), mClients.end (), [inClientId](const std::unique_ptr<Client>& client) {
        return client->GetId () == inClientId;
    });
    if (clientIterator != mClients.end ()) {
        (*clientIterator)->Send (inBuffer);
    }
    mMutex.unlock ();
};

// called from listener thread
void TCP::Server::RegisterClient (std::unique_ptr <Core::Socket> inClientSocket) {
    mMutex.lock ();
    mClients.emplace_back (std::make_unique <Client> (mRouter, std::move (inClientSocket)));
    mClients.back ()->Spawn ();
    mMutex.unlock ();
}

// called from cleaner thread
void TCP::Server::CleanUp () {
    mMutex.lock ();
    for (auto it = mClients.begin (); it != mClients.end (); it++) {
        if ((*it)->GetStatus () == Core::Thread::kDone) {
            (*it)->Join ();
            mClients.erase (it);
            break;
        }
    }
    mMutex.unlock ();
}
