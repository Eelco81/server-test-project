
#include "SystemRouter.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpEndPoint.h"

namespace {

class EchoEndPoint : public HTTP::EndPoint {
public:
    EchoEndPoint () : 
        HTTP::EndPoint ("/system/echo", HTTP::Method::PUT)
    {
    }
    
    virtual void Execute (const HTTP::Request& inRequest, HTTP::Response& outResponse) override {
        outResponse.mCode = HTTP::Code::OK;
        outResponse.mBody = inRequest.mBody;
        outResponse.mHeaders[HTTP::Header::CONTENT_LENGTH] = std::to_string (outResponse.mBody.size ());
        // outResponse.mHeaders[HTTP::Header::CONTENT_TYPE] = inRequest.mHeaders.at (HTTP::Header::CONTENT_TYPE);
    }
    
};

} // end anonymous namespace


API::SystemRouter::SystemRouter () :
    HTTP::Router ()
{
    AddEndPoint (std::make_unique<EchoEndPoint> ());
}

API::SystemRouter::~SystemRouter () = default;