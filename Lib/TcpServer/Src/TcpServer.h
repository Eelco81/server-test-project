
#ifndef _Server_H_
#define _Server_H_

#include <vector>
#include <memory>
#include "Macros.h"

namespace OS {
    class Thread;
    class Socket;
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
     * Get the number of connected clients
     */
    std::size_t GetClientCount () const;
    
    /**
     & This function is called by the listener thread
     * // todo: this should be private
     */
    void RegisterClient (std::unique_ptr <OS::Socket> inClientSocket);
    
    /**
     & This function is called by the cleanup thread
     * // todo: this should be private
     */
    bool CleanUp ();

private:
    using ClientPtr = std::unique_ptr <Client>;
    std::vector <ClientPtr> mClients;
    std::unique_ptr <OS::Mutex> mMutex;
    std::unique_ptr <OS::Thread> mListener;
    std::unique_ptr <OS::Thread> mCleaner;
    std::shared_ptr <ClientFactory> mFactory;

};

} // end namespace TCP

#endif // _Server_H_
