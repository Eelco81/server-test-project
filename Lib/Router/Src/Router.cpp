
#include "Codes.h"
#include "Router.h"
#include "Request.h"
#include "Response.h"
#include "EndPoint.h"

#include <algorithm>

Api::Router::Router () {
}

Api::Router::~Router () {
}

void Api::Router::AddEndPoint (std::unique_ptr <EndPoint> inEndPoint) {
    mEndPoints.emplace_back (std::move (inEndPoint));
}

void Api::Router::Dispatch (const Request& inRequest, Response& outResponse) {
    outResponse.SetHeader (inRequest.GetHeader ());

    auto endpointIterator = std::find_if (mEndPoints.begin (), mEndPoints.end (), [inRequest](const std::unique_ptr<EndPoint>& endpoint) {
        return endpoint->GetPath () == inRequest.GetHeader ().mPath && endpoint->GetMethod () == inRequest.GetHeader ().mMethod;
    });

    if (endpointIterator != mEndPoints.end ()) {
        (*endpointIterator)->Execute (inRequest, outResponse);
    }
    else {
        outResponse.SetCode (Codes::kNotFound);
    }
}