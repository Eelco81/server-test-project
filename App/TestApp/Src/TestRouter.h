
#ifndef _TESTROUTER_H_
#define _TESTROUTER_H_

#include "HttpRouter.h"
#include "TestService.h"

namespace TEST {

/**
 * The TEST::Router fires the streaming service.
 */
class Router : public HTTP::Router {
    
public:
    /**
     * Constructor
     */
    Router (std::shared_ptr<Service> inService);

private:
    std::shared_ptr<Service> mService;
};

}

#endif // _TESTROUTER_H_
