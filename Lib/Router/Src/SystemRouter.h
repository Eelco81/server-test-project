
#ifndef _SYSTEMROUTER_H_
#define _SYSTEMROUTER_H_

#include "HttpRouter.h"

namespace API {

class SystemRouter : public HTTP::Router {
    
public:
    SystemRouter ();
    ~SystemRouter ();
    
};

}

#endif // _SYSTEMROUTER_H_
