
#ifndef _Server_H_
#define _Server_H_

#include <vector>
#include <memory>
#include <mutex>

#include "Macros.h"

namespace OS {
    class Buffer;
    class Thread;
    class Socket;
}

namespace API {
    class Router;
    class Request;
}

namespace TCP {

class Client;

class Server {

    NO_COPY_CONSTRUCTORS (Server);

public:
    Server () = delete;
    Server (const std::string& inAddress, const std::string& inPort, std::shared_ptr <API::Router> inRouter);
    virtual ~Server ();

public:
    void Start ();
    void RegisterClient (std::unique_ptr <OS::Socket> inClientSocket);
    void CleanUp ();

    void BroadCast (const OS::Buffer& inBuffer);
    void Send (unsigned inClientId, const OS::Buffer& inBuffer);
    
private:

    using  ClientPtr = std::unique_ptr <Client>;
    std::vector <ClientPtr> mClients;

    std::shared_ptr <API::Router> mRouter;

    std::unique_ptr <OS::Thread> mListener;
    std::unique_ptr <OS::Thread> mCleaner;
    std::mutex mMutex;
};

}

#endif
