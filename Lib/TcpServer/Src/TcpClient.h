
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
    /**
     * Deleted default constructor
     */
    Client () = delete;
    
    /**
     * Create a HTTP client, which connects to inAddress:inPort
     */
    Client (std::string inAddress, std::string inPort);
    
    /**
     * Create a HTTP client out of a connected socket. The TCP::Server will
     * create these through the factory.
     */
    Client (std::unique_ptr <OS::Socket> inSocket);
    
    /**
     * Virtual destructor
     */
    virtual ~Client ();

public:
    /**
     * Start the client (messages on the port will be processed).
     */
    bool Start ();
    
    /**
     * Stop the client thread. This function should not be called from within the receive
     * handler. 
     */
    void Stop ();
    
    /**
     * Stop the client thread. This method can be called inside the receive handler. 
     * It will close the message handler and wait for the TCP::Servers cleaner to kill it.
     */
    void Quit ();
    
    /**
     * Verify if the underlying socket is connected.
     */
    bool IsConnected () const;
    
    /**
     * Send data over the socket.
     */
    bool Send (const std::vector<uint8_t>& inBuffer);
    
    /**
     * Send data over the socket.
     */
    bool Send (const uint8_t* inData, std::size_t inSize);
    
    /**
     * Get the socket id.
     */
     int GetId () const;

public: 
    /**
     * Message handler. Called when new data is available.
     * \todo: this should be private
     */
    virtual void HandlePacket (const std::vector<uint8_t>& inBuffer) = 0;
    
protected:
    std::shared_ptr <OS::Socket> mSocket;
    std::unique_ptr <OS::Thread> mThread;
};

}

#endif
