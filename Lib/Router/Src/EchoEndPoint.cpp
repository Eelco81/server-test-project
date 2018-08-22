
#include "EchoEndPoint.h"

API::EchoEndPoint::EchoEndPoint () : 
    HTTP::EndPoint ("/api/echo", HTTP::Method::PUT)
{
}

API::EchoEndPoint::~EchoEndPoint () = default;

void API::EchoEndPoint::Execute (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    outResponse.mCode = HTTP::Code::OK;
    outResponse.SetBody (inRequest.mBody);
}
