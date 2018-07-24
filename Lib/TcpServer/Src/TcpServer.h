
#ifndef _Server_H_
#define _Server_H_

#include <vector>
#include <memory>
#include <mutex>
#include "Macros.h"

namespace OS {
    class Thread;
    class Socket;
}

namespace TCP {

class Client;
class ClientFactory;

class Server {

    NO_COPY_CONSTRUCTORS (Server);

public:
    Server () = delete;
    Server (const std::string& inAddress, const std::string& inPort, std::unique_ptr<ClientFactory> inFactory);
    virtual ~Server ();

public:
    void Start ();
    void Stop ();

public:
    std::size_t GetClientCount () const;
    
public:
    void RegisterClient (std::unique_ptr <OS::Socket> inClientSocket);
    void CleanUp ();

private:
    using ClientPtr = std::unique_ptr <Client>;
    std::vector <ClientPtr> mClients;
    std::mutex mMutex;

    std::unique_ptr <OS::Thread> mListener;
    std::unique_ptr <OS::Thread> mCleaner;

    std::unique_ptr <ClientFactory> mFactory;
};

}

#endif
