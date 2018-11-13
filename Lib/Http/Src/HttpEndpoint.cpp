
#include "HttpEndpoint.h"

HTTP::EndPoint::EndPoint (const std::string& inPath, HTTP::Method inMethod) :
    mPath (inPath),
    mMethod (inMethod)
{
}

HTTP::EndPoint::~EndPoint () = default;
