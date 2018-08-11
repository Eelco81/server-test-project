
#ifndef _HTTP_CLIENT_H_
#define _HTTP_CLIENT_H_

#include <string>
#include <memory>

#include "TcpClient.h"
#include "TcpClientFactory.h"
#include "HttpRequest.h"

namespace OS {
    class Socket;
}

namespace HTTP {

struct Response;

class Client : public TCP::Client, public RequestParser {

public:
    Client () = delete;
    Client (std::string inAddress, std::string inPort);
    Client (std::unique_ptr <OS::Socket> inSocket);
    virtual ~Client () {}
    
public: // todo: this should be private
    virtual void OnReceived (const std::vector<uint8_t>& inBuffer) override;
    virtual void HandleRequest (const Request& inRequest) override;

public:
    void SendResponse (const Request& inRequest, const Response& inResponse);
};
    
class ClientFactory : public TCP::ClientFactory {
public:
    std::unique_ptr<TCP::Client> Create (std::unique_ptr<OS::Socket> inSocket) const override;
};

} // end namespace HTTP

#endif // _HTTP_CLIENT_H_
