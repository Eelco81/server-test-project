
#ifndef _HTTP_CLIENT_H_
#define _HTTP_CLIENT_H_

#include <string>
#include <memory>

#include "TcpClient.h"
#include "TcpClientFactory.h"
#include "HttpRequestDecoder.h"
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
    Client () = delete;
    Client (std::unique_ptr <OS::Socket> inSocket, std::shared_ptr<HTTP::Router> inRouter);
    virtual ~Client ();
    
public: // todo: this should be private
    virtual void HandleRequest (const Request& inRequest);

public:
    void SendResponse (const Request& inRequest, const Response& inResponse);
    
protected:
    std::shared_ptr<Router> mRouter;
    OS::PacketToStringConverter mConverter;
    RequestDecoder mDecoder;
};

class ClientFactory : public TCP::ClientFactory {
public:
    ClientFactory (std::shared_ptr<Router> inRouter);
    std::unique_ptr<TCP::Client> Create (std::unique_ptr<OS::Socket> inSocket) const override;
private:
    std::shared_ptr<Router> mRouter;
};

} // end namespace HTTP

#endif // _HTTP_CLIENT_H_
