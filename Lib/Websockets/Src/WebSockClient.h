
#ifndef _RFC6455_CLIENT_H_
#define _RFC6455_CLIENT_H_

#include <string>
#include <memory>

#include "TcpClient.h"
#include "TcpClientFactory.h"
#include "HttpRequest.h"
#include "WebSockFrameParser.h"

namespace OS {
    class Socket;
}

namespace HTTP {
    class Response;
}

namespace RFC6455 {

class Frame;

class Client : public TCP::Client, public HTTP::RequestParser, public FrameParser {

public:
    /**
     * Deleted default constructor
     */
    Client () = delete;
    
    /**
     * Constructor
     */
    Client (std::unique_ptr <OS::Socket> inSocket);
    
    /**
     * Virtual destructor
     */
    virtual ~Client ();
    
    /**
     * Process an incoming TCP Packet (from TCP::Client)
     */
    virtual void HandlePacket (const std::vector<uint8_t>& inBuffer) override;
    
    /**
     * Process an incoming handshake HTTP Request (from HTTP::RequestParser)
     */
    virtual void HandleRequest (const HTTP::Request& inRequest) override;
    
    /**
     * Process an incoming RFC6455 Frame (from RFC6455::FrameParser)
     */
    virtual void HandleFrame (const Frame& inFrame) override;
    
    /**
     * Send a frame over the RFC6455 connection
     */
    void SendFrame (const Frame& inFrame);

private:
    /**
     * Sends the websocket handshake response 
     */
    void SendResponse (const HTTP::Request& inRequest, const HTTP::Response& inResponse);
    
private:
    bool mIsUpgraded;
};

/**
 * Factory implementation
 */
class ClientFactory : public TCP::ClientFactory {
public:
    std::unique_ptr<TCP::Client> Create (std::unique_ptr<OS::Socket> inSocket) const override;
};

} // end namespace RFC6455

#endif //  _RFC6455_CLIENT_H_
