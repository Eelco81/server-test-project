
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

void HTTP::Response::SetBody (const std::string& inBody, const std::string& inContentType) {
    mBody = inBody;
    mHeaders[HTTP::Header::CONTENT_LENGTH] = std::to_string (mBody.size ());
    mHeaders[HTTP::Header::CONTENT_TYPE] = inContentType;
}

void HTTP::Response::SetBody (const std::vector<uint8_t>& inBuffer, const std::string& inContentType) {
    mBody.assign (reinterpret_cast<const char*> (inBuffer.data ()), inBuffer.size ());
    mHeaders[HTTP::Header::CONTENT_LENGTH] = std::to_string (mBody.size ());
    mHeaders[HTTP::Header::CONTENT_TYPE] = inContentType;
}

const std::string& HTTP::Response::GetBody () const {
    return mBody;
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

