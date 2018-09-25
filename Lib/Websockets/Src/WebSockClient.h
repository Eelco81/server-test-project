
#ifndef _RFC6455_CLIENT_H_
#define _RFC6455_CLIENT_H_

#include "TcpClient.h"

namespace OS {
    class Socket;
}

namespace HTTP {
    struct Request;
}

namespace RFC6455 {

class Client : public TCP::Client {

public:
    Client () = delete;
    Client (std::unique_ptr <OS::Socket> inSocket);
    virtual ~Client ();
    
public: 
    virtual void OnReceived (const std::vector<uint8_t>& inBuffer) override;
    
    virtual void HandleMessage (const Message& inMessage) override;
    virtual void SendMessage (const Message& inMessage);
    
};

class ClientFactory : public TCP::ClientFactory {
public:
    std::unique_ptr<TCP::Client> Create (std::unique_ptr<OS::Socket> inSocket) const override;
};

} // end namespace RFC6455

#endif //  _RFC6455_CLIENT_H_
