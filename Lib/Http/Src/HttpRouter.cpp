
#include <iostream>
#include <algorithm>
#include <regex>

#include "Log.h"
#include "HttpCodes.h"
#include "HttpRouter.h"

HTTP::Router::Router () :
    OS::MessageStream<HTTP::Request, HTTP::Response> ()
{
}

HTTP::Router::Router (const Router& inRouter) = default;
HTTP::Router::~Router () = default;

void HTTP::Router::AddEndPoint (HTTP::Router::EndPointPtr inEndPoint) {
    mEndPoints.push_back (inEndPoint);
}

void HTTP::Router::Write (const HTTP::Request& inRequest) {
    
    HTTP::Response response (Code::UNKNOWN_CODE, inRequest.mVersion);
    
    if (inRequest.mVersion != Version::V11) {
        response.mCode = Code::HTTP_VERSION_NOT_SUPPORTED;
    }
    else if (!inRequest.mIsValid) {
        response.mCode = Code::BAD_REQUEST;
    }
    else if (inRequest.mMethod == Method::UNKNOWN_METHOD) {
        response.mCode = Code::NOT_FOUND;
    }
    else {
        response.mCode = Code::NOT_FOUND;
        for (auto& endpoint : mEndPoints) {
            if (endpoint->Route (inRequest, response)) {
                break;
            }
        }
    }
    
    LOGINFO << "HTTP/" << VersionToString (response.mVersion) << " " << MethodToString (inRequest.mMethod) 
            << " " << inRequest.mPath << " - " << response.mCode << " " << CodeToString (response.mCode);

    sNext.Emit (response);
}
