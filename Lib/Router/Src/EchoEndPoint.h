
#ifndef _ECHOENDPOINT_H_
#define _ECHOENDPOINT_H_

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpEndPoint.h"

namespace API {

class EchoEndPoint : public HTTP::EndPoint {

public:
    EchoEndPoint ();
    virtual ~EchoEndPoint ();

    virtual void Execute (const HTTP::Request& inRequest, HTTP::Response& outResponse) override;
};

}

#endif // _ECHOENDPOINT_H_
