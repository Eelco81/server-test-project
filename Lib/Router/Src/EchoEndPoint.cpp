
#include "EchoEndPoint.h"

API::EchoEndPoint::EchoEndPoint () : 
    HTTP::EndPoint ("/system/echo", HTTP::Method::PUT)
{
}

API::EchoEndPoint::~EchoEndPoint () = default;

void API::EchoEndPoint::Execute (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    outResponse.mCode = HTTP::Code::OK;
    outResponse.mBody = inRequest.mBody;
    outResponse.mHeaders[HTTP::Header::CONTENT_LENGTH] = std::to_string (outResponse.mBody.size ());
}
