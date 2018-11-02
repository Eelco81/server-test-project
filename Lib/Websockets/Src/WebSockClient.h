
#ifndef _RFC6455_CLIENT_H_
#define _RFC6455_CLIENT_H_

#include <string>
#include <memory>

#include "TcpClient.h"
#include "TcpClientFactory.h"
#include "HttpRequestDecoder.h"
#include "HttpResponseEncoder.h"
#include "StringConverter.h"
#include "WebSockFrameDecoder.h"
#include "WebSockFrameEncoder.h"
#include "WebSockPayloadDecoder.h"
#include "WebSockPayloadEncoder.h"

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
     * Send data over the RFC6455 connection
     */
    void SendData (const std::vector<uint8_t>& inData);

    /**
     * Send data over the RFC6455 connection
     */
    virtual void SendData (const std::string& inData) override;

private:
    /**
     * Process an incoming handshake HTTP Request (from HTTP::RequestDecoder)
     */
    virtual void HandleHandshake (const HTTP::Request& inRequest);
    
    /**
     * Process an incoming RFC6455 Frame (from RFC6455::FrameDecoder)
     */
    virtual void HandleReceivedFrame (const Frame& inFrame);
    
private:
    OS::PacketToStringConverter mToStringConverter;
    OS::StringToPacketConverter mToPacketConverter;
    HTTP::RequestDecoder mRequestDecoder;
    HTTP::ResponseEncoder mResponseEncoder;
    FrameDecoder mFrameDecoder;
    FrameEncoder mFrameEncoder;
    PayloadStringDecoder mPayloadStringDecoder;
    PayloadBinaryDecoder mPayloadBinaryDecoder;
    PayloadStringEncoder mPayloadStringEncoder;
    PayloadBinaryEncoder mPayloadBinaryEncoder;
    
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
