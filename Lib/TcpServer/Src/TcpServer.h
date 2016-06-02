
#ifndef _Server_H_
#define _Server_H_

#include <vector>
#include <memory>
#include <mutex>

namespace Core {
    class Buffer;
    class Thread;
    class Socket;
}

namespace Api {

class Client;
class Router;
class Request;

class Server {

public:
    Server () = delete;
    Server (const Server& inServer) = delete;
    Server& operator= (const Server& inServer) = delete;

    Server (const std::string& inAddress, const std::string& inPort);
    virtual ~Server ();

public:
    void Start ();
    void RegisterClient (std::unique_ptr <Core::Socket> inClientSocket);
    void CleanUp ();

    void BroadCast (const Core::Buffer& inBuffer);
    void Send (unsigned inClientId, const Core::Buffer& inBuffer);
    
private:
    std::unique_ptr <Core::Thread> mListener;
    std::unique_ptr <Core::Thread> mCleaner;
    std::vector <std::unique_ptr <Client>> mClients;
    std::mutex mMutex;
    std::shared_ptr <Router> mRouter;
};

}

#endif
