
#include "HttpResponse.h"

HTTP::Response::Response () :
    mCode (Code::UNKNOWN_CODE),
    mVersion (Version::UNKNOWN_VERSION)
{
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