
#ifndef _HTTP_CLIENT_H_
#define _HTTP_CLIENT_H_

#include <string>
#include <memory>

#include "TcpClient.h"
#include "TcpClientFactory.h"
#include "HttpRequestDecoder.h"
#include "HttpResponseEncoder.h"
#include "HttpRouter.h"
#include "StringConverter.h"

namespace OS {
    class Socket;
}

namespace HTTP {

struct Request;
struct Response;

/**
 * The HTTP::Client class routes HTTP Requests on an open TCP socket to 
 * a router and forwards the resulting HTTP Responses to the same socket.
 */
class Client : public TCP::Client {

public:
    /**
     * Constructor
     */
    Client (std::unique_ptr <OS::Socket> inSocket, const Router& inRouter);
    
    /**
     * Virtual destructor
     */
    virtual ~Client ();
    
protected:
    /**
     * MessageStreams which link ReadStream to the WriteStream
     */
    Router mRouter;
    OS::PacketToStringConverter mToStringConverter;
    OS::StringToPacketConverter mToPacketConverter;
    RequestDecoder mDecoder;
    ResponseEncoder mEncoder;
};

class ClientFactory : public TCP::ClientFactory {

public:
    /**
     * HTTP ClientFactory
     */
    ClientFactory (const Router& inRouter);
    
    /**
     * Overloaded create method
     */
    std::unique_ptr<TCP::Client> Create (std::unique_ptr<OS::Socket> inSocket) const override;

private:
    /**
     * The router is copied to each created client
     */
    Router mRouter;

};

} // end namespace HTTP

#endif // _HTTP_CLIENT_H_
