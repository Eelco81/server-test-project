
#include <iostream>
#include "HttpCodes.h"
#include "HttpRouter.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpEndpoint.h"

#include <iostream>
#include <algorithm>
#include "HttpEndpoint.h"

HTTP::Router::Router () = default;
HTTP::Router::~Router () = default;

void HTTP::Router::AddEndPoint (std::unique_ptr <HTTP::EndPoint> inEndPoint) {
    std::lock_guard<std::mutex> lockGuard (mMutex);
    mEndPoints.push_back (std::move (inEndPoint));
}

void HTTP::Router::Dispatch (const HTTP::Request& inRequest, HTTP::Response& outResponse) {

    std::lock_guard<std::mutex> lockGuard (mMutex);
    
    outResponse.mVersion = inRequest.mVersion;
    outResponse.mCode = Code::INTERNAL_SERVER_ERROR;
    
    if (inRequest.mVersion != Version::V11) {
        outResponse.mCode = Code::HTTP_VERSION_NOT_SUPPORTED;
        return;
    }
    
    if (!inRequest.mIsValid) {
        outResponse.mCode = Code::BAD_REQUEST;
        return;
    }
    
    auto endpointIterator = std::find_if (mEndPoints.begin (), mEndPoints.end (), [&inRequest](const std::unique_ptr<EndPoint>& endpoint) {
        return endpoint->GetPath () == inRequest.mPath && endpoint->GetMethod () == inRequest.mMethod;
    });

    if (endpointIterator != mEndPoints.end ()) {
        (*endpointIterator)->Execute (inRequest, outResponse);
    }
    else {
        outResponse.mCode = Code::NOT_FOUND;
    }
}
