
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
        ListenThread (Api::Server& inServer, const std::string& inAddress, const std::string& inPort) :
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
        Api::Server& mServer;
        Core::Socket mSocket;
    };

    class CleanUpTask : public Core::Task {
    public:
        CleanUpTask (Api::Server& inServer) : 
            Task ("CleanupServerTask"),
            mServer (inServer) 
        {
        }
        virtual bool Step () override {
            mServer.CleanUp ();
            return true;
        }
    private:
        Api::Server& mServer;
    };

    class CleanupThread : public Core::PeriodicThread {
    public:
        CleanupThread (Api::Server& inServer) :
            PeriodicThread ("CleanupServer", 1000u)
        {
            AddTask (std::make_unique <CleanUpTask> (inServer));
        }
        ~CleanupThread () {}
    };

}

Api::Server::Server (const std::string& inAddress, const std::string& inPort) : 
    mListener (std::make_unique <ListenThread> (*this, inAddress, inPort)),
    mCleaner (std::make_unique <CleanupThread> (*this)),
    mRouter (std::make_shared <Router> ())
{
}

Api::Server::~Server () {
}

void Api::Server::Start () {
    mListener->Spawn ();
    mCleaner->Spawn ();
}

void Api::Server::BroadCast (const Core::Buffer& inBuffer) {
    mMutex.lock ();
    std::for_each (mClients.begin (), mClients.end (), [&](std::unique_ptr<Client>& client) {
        client->Send (inBuffer);
    });
    mMutex.unlock ();
};

void Api::Server::Send (unsigned inClientId, const Core::Buffer& inBuffer) {
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
void Api::Server::RegisterClient (std::unique_ptr <Core::Socket> inClientSocket) {
    mMutex.lock ();
    mClients.emplace_back (std::make_unique <Client> (mRouter, std::move (inClientSocket)));
    mClients.back ()->Spawn ();
    mMutex.unlock ();
}

// called from cleaner thread
void Api::Server::CleanUp () {
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
