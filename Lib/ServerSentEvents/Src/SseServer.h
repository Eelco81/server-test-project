
#ifndef _SSE_SERVER_H_
#define _SSE_SERVER_H_

#include <string>
#include <cstdint>
#include <vector>

#include "TcpServer.h"

namespace SSE {

/**
 * The SSE server hosts Server-Sent Event connections.
 */
class Server : public TCP::Server {
    
public:
    
    /**
     * Create a server which listen to inAddress::inPort and accepts incoming socket connections.
     * It will spawn a SSE::Client instance generated through the ClientFactory.
     */
    Server (const std::string& inAddress, const std::string& inPort);
    
    /**
     * Virtual destructor
     */
    virtual ~Server ();

    /**
     * Send a packet to all connected clients (TEXT)
     */
    void BroadCast (const std::string& inPacket);
};

} // end namespace SSE

#endif // _SSE_SERVER_H_
