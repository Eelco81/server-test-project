
#ifndef _HTTP_CLIENT_H_
#define _HTTP_CLIENT_H_

#include <string>
#include <memory>

#include "TcpClient.h"
#include "TcpClientFactory.h"
#include "HttpRequestDecoder.h"
#include "HttpResponseEncoder.h"
#include "StringConverter.h"

namespace OS {
    class Socket;
}

namespace HTTP {

struct Request;
struct Response;
class Router;

class Client : public TCP::Client {

public:
    /**
     * Deleted default constructor
     */
    Client () = delete;
    
    /**
     * Constructor
     */
    Client (std::unique_ptr <OS::Socket> inSocket, std::shared_ptr<HTTP::Router> inRouter);
    
    /**
     * Virtual destructor
     */
    virtual ~Client ();
    
protected:
    /**
     * MessageStreams which link ReadStream to the WriteStream
     */
    std::shared_ptr<Router> mRouter;
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
    ClientFactory (std::shared_ptr<Router> inRouter);
    
    /**
     * Overloaded create method
     */
    std::unique_ptr<TCP::Client> Create (std::unique_ptr<OS::Socket> inSocket) const override;

private:
    /**
     * The router is shared with each create client
     */
    std::shared_ptr<Router> mRouter;

};

} // end namespace HTTP

#endif // _HTTP_CLIENT_H_
