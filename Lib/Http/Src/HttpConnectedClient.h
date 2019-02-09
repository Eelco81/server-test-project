

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

class Client : public TCP::Client {

public:
    /**
     * Deleted default constructor
     */
    Client () = delete;
    
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
    OS::PacketToStringConverter mToStringConverter;
    OS::StringToPacketConverter mToPacketConverter;
    RequestEcoder mDecoder;
    ResponseEncoder mEncoder;
};

class ClientFactory : public TCP::ClientFactory {

public:

} // end namespace HTTP

#endif // _HTTP_CLIENT_H_
