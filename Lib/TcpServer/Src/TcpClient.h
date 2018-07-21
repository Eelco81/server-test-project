
#ifndef _Client_H_
#define _Client_H_

#include "Macros.h"
#include "Thread.h"

#include <memory>
#include <vector>

namespace OS {
    class Buffer;
    class Socket;
}

namespace TCP {

class Client : public OS::Thread {

    NO_COPY_CONSTRUCTORS (Client);

public:
    Client () = delete;
    Client (std::unique_ptr <OS::Socket> inSocket);
    virtual ~Client ();

public:
    virtual void Execute () override;
    virtual void Kill() override;
    
    void Send (const OS::Buffer& inBuffer);
    virtual void OnReceived (const OS::Buffer& inBuffer) = 0;
    
    unsigned GetId () const;

private:
    std::unique_ptr <OS::Socket> mSocket;
    std::atomic<bool> mKeepAlive;
};

}

#endif
