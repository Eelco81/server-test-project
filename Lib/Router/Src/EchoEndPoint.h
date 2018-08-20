
#ifndef _ECHOENDPOINT_H_
#define _ECHOENDPOINT_H_

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpEndPoint.h"

namespace API {

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
    }
};

}

#endif // _ECHOENDPOINT_H_
