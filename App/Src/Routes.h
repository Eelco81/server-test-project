
#ifndef _ROUTES_H_
#define _ROUTES_H_

#include <memory>

namespace API {
    class Router;
}

class Routes {
public:
    static void SetupEndpoints (std::shared_ptr <API::Router> ioRouter);
};


#endif
