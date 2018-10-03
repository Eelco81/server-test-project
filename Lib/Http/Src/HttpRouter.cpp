
#include <iostream>
#include <algorithm>

#include "Log.h"
#include "HttpCodes.h"
#include "HttpRouter.h"

HTTP::Router::~Router () = default;

void HTTP::Router::AddEndPoint (HTTP::Router::EndPointPtr inEndPoint) {
    OS::SingleLock lock (mMutex);
    mEndPoints.push_back (std::move (inEndPoint));
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
        OS::SingleLock lock (mMutex);
        
        auto endpointIterator = std::find_if (mEndPoints.begin (), mEndPoints.end (), [&inRequest](const std::unique_ptr<EndPoint>& endpoint) {
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
    
    Done (response);
    
    LOGMESSAGE (OS::Log::kInfo, std::string ("HTTP/") + VersionToString (response.mVersion) + std::string (" ") + MethodToString (inRequest.mMethod) + std::string (" ") + inRequest.mPath + std::string (" - ") + std::to_string (response.mCode) + std::string (" ") + CodeToString (response.mCode));

}
