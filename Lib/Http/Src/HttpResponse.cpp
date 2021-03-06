
#include "HttpResponse.h"
#include "Timing.h"
#include "Version.h"

HTTP::Response::Response () :
    Message (),
    mCode (Code::UNKNOWN_CODE),
    mVersion (Version::UNKNOWN_VERSION)
{
}

HTTP::Response::Response (HTTP::Code inCode, HTTP::Version inVersion) :
    Message (),
    mCode (inCode),
    mVersion (inVersion)
{
    SetHeader (Header (Header::LAST_MODIFIED, OS::Timing::ToStdString (OS::Timing::Now ())));
    SetHeader (Header (Header::USER_AGENT, OS::Version::GetApplicationName () + std::string ("/") + OS::Version::GetApplicationVersion ()));
}
