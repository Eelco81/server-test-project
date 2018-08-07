
#include "HttpResponse.h"

HTTP::Response::Response () :
    mCode (Code::UNKNOWN_CODE),
    mVersion (Version::UNKNOWN_VERSION)
{
}

std::string HTTP::Response::ToString () const {
    auto response (std::string ("HTTP/") + VersionToString (mVersion) + std::string (" ") + std::to_string (mCode) + std::string (" ") + CodeToString (mCode) + std::string ("\n"));
    for (const auto& pair : mHeaders) {
        response += pair.first + std::string (": ") + pair.second + std::string ("\n");
    }
    response += std::string ("\n");
    response += mBody;
    return response;
}