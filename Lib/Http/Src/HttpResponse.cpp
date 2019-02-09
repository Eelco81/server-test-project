
#include "HttpResponse.h"
#include "Timing.h"
#include "Version.h"

HTTP::Response::Response () :
    HTTP::Response::Response (Code::UNKNOWN_CODE, Version::UNKNOWN_VERSION) 
{
}

HTTP::Response::Response (HTTP::Code inCode, HTTP::Version inVersion) :
    Message (),
    mCode (inCode),
    mVersion (inVersion)
{
    mHeaders[Header::LAST_MODIFIED] = OS::Timing::ToStdString (OS::Timing::Now ());
    mHeaders[Header::USER_AGENT] = OS::Version::GetApplicationName () + std::string ("/") + OS::Version::GetApplicationVersion ();
}

