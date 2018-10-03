
#ifndef _HTTP_ROUTER_H_
#define _HTTP_ROUTER_H_

#include <memory>
#include <vector>

#include "Macros.h"
#include "Mutex.h"
#include "MessageStream.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpEndpoint.h"

namespace HTTP {

class Router : public OS::MessageStream <Request, Response> {
    
public:
    using EndPointPtr = std::unique_ptr <EndPoint>;
    Router ();
    virtual ~Router ();
    
public:
    void Write (const Request& inRequest) override;
    void AddEndPoint (EndPointPtr inEndPoint);

private:
    std::vector <EndPointPtr> mEndPoints;
    OS::Mutex mMutex;
};

}

#endif // _HTTP_ROUTER_H_
