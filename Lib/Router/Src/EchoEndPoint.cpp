
#include "EchoEndPoint.h"

API::EchoEndPoint::EchoEndPoint (const std::string& inPath) : 
    HTTP::EndPoint (inPath, HTTP::Method::PUT)
{
}

API::EchoEndPoint::~EchoEndPoint () = default;

void API::EchoEndPoint::Execute (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    outResponse.mCode = HTTP::Code::OK;
    outResponse.SetBody (inRequest.mBody);
}
