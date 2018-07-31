
#ifndef _Client_H_
#define _Client_H_

#include "Macros.h"
#include "Thread.h"

#include <cstdint>
#include <memory>
#include <vector>
#include <string>

namespace OS {
    class Socket;
    class Thread;
}

namespace TCP {

class Client {

    NO_COPY_CONSTRUCTORS (Client);

public: 
    Client () = delete;
    Client (std::string inAddress, std::string inPort);
    Client (std::unique_ptr <OS::Socket> inSocket);
    virtual ~Client ();

public:
    bool Start ();
    void Stop ();
    bool IsConnected () const;
    bool Send (const std::vector<uint8_t>& inBuffer);
    int GetId () const;

public: // todo: this should be private
    virtual void OnReceived (const std::vector<uint8_t>& inBuffer) = 0;
    
private:
    std::shared_ptr <OS::Socket> mSocket;
    std::unique_ptr <OS::Thread> mThread;
};

}

#endif
