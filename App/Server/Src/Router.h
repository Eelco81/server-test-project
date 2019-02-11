
#ifndef _SYSTEMROUTER_H_
#define _SYSTEMROUTER_H_

#include "HttpRouter.h"

namespace SIM {
    class Service;
}

class SystemRouter : public HTTP::Router {
    
public:
    SystemRouter (std::shared_ptr<SIM::Service> inService);

};

#endif // _SYSTEMROUTER_H_
