
#ifndef _SSE_CLIENT_H_
#define _SSE_CLIENT_H_

#include <string>
#include <memory>

#include "TcpClient.h"
#include "TcpClientFactory.h"
#include "TcpPacketEncoders.h"
#include "HttpRequestDecoder.h"
#include "HttpResponseEncoder.h"
#include "SseEventEncoder.h"

namespace OS {
    class Socket;
}

namespace HTTP {
    class Request;
    class Response;
}

namespace SSE {

/**
 * The SSE::Client is a TCP::Client which is launched by the SSE::Server when a TCP connection
 * is accepted. This client enables Server-Sent-Events connections. 
 */
class Client : public TCP::Client {

public:
    /**
     * Constructor
     */
    Client (std::unique_ptr <OS::Socket> inSocket);
    
    /**
     * Virtual destructor
     */
    virtual ~Client ();
    
    /**
     * Send data over the SSE connection
     */
    virtual void SendPayload (const std::string& inData);

private:
    /**
     * Process an incoming handshake HTTP Request (from HTTP::RequestDecoder)
     */
    virtual void HandleHandshake (const HTTP::Request& inRequest);
    
private:
    TCP::Packet2StringEncoder mPacket2String;
    TCP::String2PacketEncoder mString2Packet;
    HTTP::RequestDecoder mRequestDecoder;
    HTTP::ResponseEncoder mResponseEncoder;
    EventEncoder mEventEncoder;
};

/**
 * Factory implementation
 */
class ClientFactory : public TCP::ClientFactory {
public:
    std::unique_ptr<TCP::Client> Create (std::unique_ptr<OS::Socket> inSocket) const override;
};

} // end namespace RFC6455

#endif //  _SSE_CLIENT_H_
