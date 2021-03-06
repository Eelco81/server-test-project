
#ifndef _TIMEENDPOINT_H_
#define _TIMEENDPOINT_H_

#include "HttpEndpoint.h"

namespace API {

class TimeEndPoint : public HTTP::EndPoint {

public:
    TimeEndPoint (const std::string& inPath);
    virtual ~TimeEndPoint ();

    virtual void Get (const HTTP::Request& inRequest, HTTP::Response& outResponse) override;
};

}

#endif // _TIMEENDPOINT_H_
