
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

/**
 * The HTTP::Router is a MessageStream which handles
 * an HTTP::Request and yields a HTTP::Response.
 */
class Router : public OS::MessageStream <Request, Response> {

public:
    
    /**
     * Convenience definition.
     */
    using EndPointPtr = std::shared_ptr <AbstractEndPoint>;
    
    /**
     * Copy constructor 
     */
    Router (const Router& inRouter);
    
    /**
     * Default constructor
     */
    Router ();
    
    /**
     * Virtual destructor
     */
    virtual ~Router ();
    
    /**
     * Write to a HTTP::Request to the Router, when a HTTP::Response
     * is computed, it is passed through MessageStream::Emit
     */
    void Write (const Request& inRequest) override;
    
    /**
     * Add an EndPoint to the Router
     */
    void AddEndPoint (EndPointPtr inEndPoint);

private:
    std::vector <EndPointPtr> mEndPoints;
};

}

#endif // _HTTP_ROUTER_H_
