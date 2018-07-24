
#ifndef _Client_H_
#define _Client_H_

#include "Macros.h"
#include "Thread.h"

#include <cstdint>
#include <memory>
#include <vector>

namespace OS {
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
    
    void Send (const std::vector<uint8_t>& inBuffer);
    virtual void OnReceived (const std::vector<uint8_t>& inBuffer) = 0;
    
    int GetId () const;

private:
    std::unique_ptr <OS::Socket> mSocket;
};

}

#endif
