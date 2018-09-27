
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
    Client () = delete;
    Client (std::unique_ptr <OS::Socket> inSocket);
    virtual ~Client ();
    
public: 
    virtual void OnReceived (const std::vector<uint8_t>& inBuffer) override;
    
    virtual void HandleRequest (const HTTP::Request& inRequest) override;
    virtual void HandleFrame (const Frame& inFrame) override;
    
public:
    void SendResponse (const HTTP::Request& inRequest, const HTTP::Response& inResponse);
    void SendFrame (const Frame& inFrame);
    
private:
    bool mIsUpgraded;
};

class ClientFactory : public TCP::ClientFactory {
public:
    std::unique_ptr<TCP::Client> Create (std::unique_ptr<OS::Socket> inSocket) const override;
};

} // end namespace RFC6455

#endif //  _RFC6455_CLIENT_H_
