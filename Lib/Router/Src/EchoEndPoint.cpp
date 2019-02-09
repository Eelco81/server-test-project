
#include "EchoEndPoint.h"

API::EchoEndPoint::EchoEndPoint (const std::string& inPath) : 
    HTTP::EndPoint (inPath)
{
}

API::EchoEndPoint::~EchoEndPoint () = default;

void API::EchoEndPoint::Put (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    outResponse.mCode = HTTP::Code::OK;
    outResponse.SetBody (inRequest.GetBody ());
}
