
#ifndef _Server_H_
#define _Server_H_

#include <vector>
#include <memory>
#include <mutex>

#include <atomic>

#include "Macros.h"

namespace OS {
    class Buffer;
    class Thread;
    class Socket;
}

namespace TCP {

class Client;

class Server {

    NO_COPY_CONSTRUCTORS (Server);

public:
    Server () = delete;
    Server (const std::string& inAddress, const std::string& inPort);
    virtual ~Server ();

public:
    void Start ();
    void Stop ();

    void BroadCast (const OS::Buffer& inBuffer);
    void Send (unsigned inClientId, const OS::Buffer& inBuffer);
    
public:
    std::size_t GetClientCount () const;
    
public:
    void RegisterClient (std::unique_ptr <OS::Socket> inClientSocket);
    void CleanUp ();

private:
    using  ClientPtr = std::unique_ptr <Client>;
    std::vector <ClientPtr> mClients;
    std::mutex mMutex;

    std::unique_ptr <OS::Thread> mListener;
    std::unique_ptr <OS::Thread> mCleaner;

};

}

#endif
