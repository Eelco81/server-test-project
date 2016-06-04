
#ifndef _Server_H_
#define _Server_H_

#include <vector>
#include <memory>
#include <mutex>

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

public:
    Server () = delete;
    Server (const Server& inServer) = delete;
    Server& operator= (const Server& inServer) = delete;

    Server (const std::string& inAddress, const std::string& inPort, std::shared_ptr <API::Router> inRouter);
    virtual ~Server ();

public:
    void Start ();
    void RegisterClient (std::unique_ptr <OS::Socket> inClientSocket);
    void CleanUp ();

    void BroadCast (const OS::Buffer& inBuffer);
    void Send (unsigned inClientId, const OS::Buffer& inBuffer);
    
private:
    std::unique_ptr <OS::Thread> mListener;
    std::unique_ptr <OS::Thread> mCleaner;
    std::vector <std::unique_ptr <Client>> mClients;
    std::mutex mMutex;
    std::shared_ptr <API::Router> mRouter;
};

}

#endif
