
#ifndef _VERSIONENDPOINT_H_
#define _VERSIONENDPOINT_H_

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpEndPoint.h"

namespace API {

class VersionEndPoint : public HTTP::EndPoint {

public:
    VersionEndPoint ();
    virtual ~VersionEndPoint ();

    virtual void Execute (const HTTP::Request& inRequest, HTTP::Response& outResponse) override;
};

}

#endif // _VERSIONENDPOINT_H_
