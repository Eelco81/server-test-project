
#include "FileEndPoint.h"
#include "ApiUtils.h"

API::FileEndPoint::FileEndPoint (const std::string& inPath, const std::string& inFileName) : 
    HTTP::EndPoint (inPath),
    mFileName (inFileName)
{
}

API::FileEndPoint::~FileEndPoint () = default;

void API::FileEndPoint::Get (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    API::Utils::SetFileMessage (outResponse, mFileName);
}

void API::FileEndPoint::Put (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    API::Utils::SetErrorMessage (outResponse, HTTP::Code::NOT_IMPLEMENTED, "Not available yet");
}
