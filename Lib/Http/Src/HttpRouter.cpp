
#include <iostream>
#include <algorithm>

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
    
    HTTP::Response response;
    response.mVersion = inRequest.mVersion;
    
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
        
        auto endpointIterator = std::find_if (mEndPoints.begin (), mEndPoints.end (), [&inRequest](const auto& endpoint) {
            return endpoint->GetPath () == inRequest.mPath && endpoint->GetMethod () == inRequest.mMethod;
        });
        
        if (endpointIterator != mEndPoints.end ()) {
            // The endppoint iterator must set the response code.
            response.mCode = Code::INTERNAL_SERVER_ERROR;
            (*endpointIterator)->Execute (inRequest, response);
        }
        else {
            response.mCode = Code::NOT_FOUND;
        }
    }
    
    LOGINFO << "HTTP/" << VersionToString (response.mVersion) << " " << MethodToString (inRequest.mMethod) 
            << " " << inRequest.mPath << " - " << response.mCode << " " << CodeToString (response.mCode);

    Done (response);
    
}
