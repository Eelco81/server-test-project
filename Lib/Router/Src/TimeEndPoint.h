

#ifndef _TIMEENDPOINT_H_
#define _TIMEENDPOINT_H_

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpEndPoint.h"

namespace API {

class TimeEndPoint : public HTTP::EndPoint {

public:
    TimeEndPoint ();
    virtual ~TimeEndPoint ();

    virtual void Execute (const HTTP::Request& inRequest, HTTP::Response& outResponse) override;
};

}

#endif // _TIMEENDPOINT_H_
