
#include "HttpResponse.h"
#include "Timing.h"
#include "Version.h"

HTTP::Response::Response () :
    HTTP::Response::Response (Code::UNKNOWN_CODE, Version::UNKNOWN_VERSION) 
{
}

HTTP::Response::Response (Code inCode, Version inVersion) :
    mCode (inCode),
    mVersion (inVersion)
{
    mHeaders[Header::CONTENT_LENGTH] = "0";
    mHeaders[Header::LAST_MODIFIED] = OS::Timing::ToStdString (OS::Timing::Now ());
    mHeaders[Header::USER_AGENT] = OS::Version::GetApplicationName () + std::string ("/") + OS::Version::GetApplicationVersion ();
}

std::string HTTP::Response::ToString () const {
    auto response (std::string ("HTTP/") + VersionToString (mVersion) + std::string (" ") + std::to_string (mCode) + std::string (" ") + CodeToString (mCode) + std::string ("\r\n"));
    for (const auto& pair : mHeaders) {
        response += pair.first + std::string (": ") + pair.second + std::string ("\r\n");
    }
    response += std::string ("\r\n");
    response += mBody;
    return response;
}
