
#ifndef _Client_H_
#define _Client_H_

#define MAX_BUFFER_SIZE 2048

#include "Thread.h"

#include <memory>
#include <vector>
#include <string>
#include <mutex>

class Core::Socket;

namespace API {
    class Router;
}

namespace TCP {

class Client : public Core::Thread {

public:
    Client () = delete;
    Client (const Client& inClient) = delete;
    Client& operator= (const Client& inClient) = delete;

    Client (std::shared_ptr <API::Router> inRouter, std::unique_ptr <Core::Socket> inSocket);
    virtual ~Client ();

public:
    virtual void Execute () override;
    
    void Send (const Core::Buffer& inBuffer);
    void ForceClose ();

    unsigned GetId () const {
        return mSocket->GetId ();
    }

private:
    std::shared_ptr <API::Router> mRouter;
    std::unique_ptr <Core::Socket> mSocket;
    std::vector <std::string> mMessages;
    std::mutex mMessagesMutex;
};

}

#endif
