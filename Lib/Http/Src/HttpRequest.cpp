
#include "HttpRequest.h"
#include "Timing.h"
#include "Version.h"

HTTP::Request::Request () :
    Message (), 
    mMethod (HTTP::Method::UNKNOWN_METHOD),
    mVersion (HTTP::Version::UNKNOWN_VERSION)
{
}

HTTP::Request::Request (HTTP::Method inMethod, const std::string& inPath) :
    HTTP::Request::Request (inMethod, inPath, HTTP::Version::V11)
{
}

HTTP::Request::Request (Method inMethod, const std::string& inPath, Version inVersion):
    Message (), 
    mMethod (inMethod),
    mVersion (inVersion),
    mPath (inPath)
{
    SetHeader (Header (Header::LAST_MODIFIED, OS::Timing::ToStdString (OS::Timing::Now ())));
    SetHeader (Header (Header::USER_AGENT, OS::Version::GetApplicationName () + std::string ("/") + OS::Version::GetApplicationVersion ()));
}
