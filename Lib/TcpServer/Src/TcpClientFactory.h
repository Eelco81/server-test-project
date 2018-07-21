
#include <memory>

namespace OS {
    class Socket;
}

namespace TCP {

class ClientFactory {

public:
    virtual std::unique_ptr<Client> Create (std::unique_ptr<OS::Socket> inSocket) = 0;

};

}


