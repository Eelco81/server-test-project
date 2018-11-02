
#ifndef _RFC6455_SERVER_H_
#define _RFC6455_SERVER_H_

#include <string>
#include <cstdint>
#include <vector>

#include "TcpServer.h"

namespace RFC6455 {

class Server : public TCP::Server {
    
public:
    
    /**
     * Create a server which listen to inAddress::inPort and accepts incoming socket connections.
     * It will spawn a RFC6455::Client instance generated through the ClientFactory.
     */
    Server (const std::string& inAddress, const std::string& inPort);
    
    /**
     * Virtual destructor
     */
    virtual ~Server ();
    
    /**
     * Send a packet to all connected clients (BINARY)
     */
    void BroadCast (const std::vector<uint8_t>& inPacket);

    /**
     * Send a packet to all connected clients (TEXT)
     */
    void BroadCast (const std::string& inPacket);
};

} // end namespace RFC6455

#endif // _RFC6455_SERVER_H_
