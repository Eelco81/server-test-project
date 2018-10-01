
#ifndef _RFC6455_CLIENT_H_
#define _RFC6455_CLIENT_H_

#include <string>
#include <memory>

#include "TcpClient.h"
#include "TcpClientFactory.h"
#include "HttpRequestDecoder.h"
#include "StringConverter.h"
#include "WebSockFrameDecoder.h"

namespace OS {
    class Socket;
}

namespace HTTP {
    class Response;
}

namespace RFC6455 {

class Frame;

class Client : public TCP::Client {

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
     * Process an incoming handshake HTTP Request (from HTTP::RequestDecoder)
     */
    virtual void HandleRequest (const HTTP::Request& inRequest);
    
    /**
     * Process an incoming RFC6455 Frame (from RFC6455::FrameDecoder)
     */
    virtual void HandleFrame (const Frame& inFrame);
    
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
    OS::PacketToStringConverter mConverter;
    HTTP::RequestDecoder mRequestDecoder;
    FrameDecoder mFrameDecoder;

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
