
#ifndef _Server_H_
#define _Server_H_

#include <vector>
#include <memory>
#include "Macros.h"
#include "Socket.h"
#include "TcpPacket.h"

namespace OS {
    class Thread;
    class Mutex;
}

namespace TCP {

class Client;
class ClientFactory;

class Server {

    NO_COPY_CONSTRUCTORS (Server);

public:
    /**
     * No default constructor
     */
    Server () = delete;
    
    /**
     * Create a server which listen to inAddress::inPort and accepts incoming socket connections.
     * It will spawn a TCP::Client instance generated through the ClientFactory.
     */
    Server (const std::string& inAddress, const std::string& inPort, std::shared_ptr<ClientFactory> inFactory);
    
    /**
     * Virtual destructor
     */
    virtual ~Server ();

    /**
     * Start the server (this will spawn a listening thread).
     */
    void Start ();
    
    /**
     * Stop the server (this will kill and join the listening thread).
     */
    void Stop ();

    /**
     * Broadcast a Packet to all connected clients
     */
    void Broadcast (const Packet& inPacket);

    /**
     * Broadcast a RawPacket to all connected clients
     */
    void Broadcast (const RawPacket& inPacket);

    /**
     * Get the number of connected clients
     */
    std::size_t GetClientCount () const;
    
    /**
     * This function is called by the listener thread
     * // todo: this should be private
     */
    void WaitForConnections ();
    
    /**
     * This function is called by the cleanup thread
     * // todo: this should be private
     */
    bool CleanUp ();

protected:
    using ClientPtr = std::unique_ptr <Client>;
    std::vector <ClientPtr> mClients;
    std::unique_ptr <OS::Mutex> mMutex;
    std::unique_ptr <OS::Thread> mListener;
    std::unique_ptr <OS::Thread> mCleaner;
    std::shared_ptr <ClientFactory> mFactory;
    OS::Socket mSocket;

};

} // end namespace TCP

#endif // _Server_H_
