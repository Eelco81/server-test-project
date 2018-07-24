
#ifndef _TCP_ClientFactory_H_
#define _TCP_ClientFactory_H_

#include <memory>

namespace OS {
    class Socket;
}

namespace TCP {

class Client;

class ClientFactory {
public:
    virtual std::unique_ptr<Client> Create (std::unique_ptr<OS::Socket> inSocket) const = 0;
};

}

#endif // _TCP_ClientFactory_H_
