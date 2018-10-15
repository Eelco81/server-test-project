
#ifndef _VERSIONENDPOINT_H_
#define _VERSIONENDPOINT_H_

#include "HttpEndPoint.h"

namespace API {

class VersionEndPoint : public HTTP::EndPoint {

public:
    VersionEndPoint (const std::string& inPath);
    virtual ~VersionEndPoint ();

    virtual void Execute (const HTTP::Request& inRequest, HTTP::Response& outResponse) override;
};

}

#endif // _VERSIONENDPOINT_H_
