
#ifndef _Client_H_
#define _Client_H_

#include "Macros.h"
#include "Thread.h"
#include "Signals.h"
#include "TcpPacket.h"

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
    void Send (const Packet& inPacket);
    
    /**
     * Send data over the socket.
     */
    void Send (const RawPacket& inPacket);

    /**
     * Get the socket id.
     */
     int GetId () const;

     /**
      * Wait for incoming data packets
      * \todo: this should be private
      */
    void WaitForData ();
    
    /**
     * This signal is emitted when data is available on the TCP connection.
     * It uses a RawPacket rather than a Packet to avoid resizing a buffer.
     */
    OS::Signal<const RawPacket&> sDataAvailable;
    
    /**
     * This signal is emitted when the connection is closed
     */
    OS::Signal<> sClosed;

private:
    std::unique_ptr <OS::Thread> mThread;
    std::shared_ptr <OS::Socket> mSocket;
};

}

#endif
