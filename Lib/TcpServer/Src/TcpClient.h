
#ifndef _Client_H_
#define _Client_H_

#define MAX_BUFFER_SIZE 2048

#include "Macros.h"
#include "Thread.h"

#include <memory>
#include <vector>
#include <string>
#include <mutex>

namespace OS {
    class Socket;
}
namespace API {
    class Router;
}

namespace TCP {

class Client : public OS::Thread {

    NO_COPY_CONSTRUCTORS (Client);

public:
    Client () = delete;
    Client (std::shared_ptr <API::Router> inRouter, std::unique_ptr <OS::Socket> inSocket);
    virtual ~Client ();

public:
    virtual void Execute () override;
    
    void Send (const OS::Buffer& inBuffer);
    void ForceClose ();

    unsigned GetId () const {
        return mSocket->GetId ();
    }

private:
    std::shared_ptr <API::Router> mRouter;
    std::unique_ptr <OS::Socket> mSocket;
};

}

#endif
