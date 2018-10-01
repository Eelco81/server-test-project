
#include "HttpRequest.h"

HTTP::Request::Request () :
    mMethod (HTTP::Method::UNKNOWN_METHOD),
    mVersion (HTTP::Version::UNKNOWN_VERSION),
    mIsValid (true)
{
}

// Mainly a convenience constructor for unit tests
HTTP::Request::Request (HTTP::Method inMethod, const std::string& inPath) :
    mMethod (inMethod),
    mVersion (HTTP::Version::V11),
    mPath (inPath),
    mIsValid (true)
{
}

std::string HTTP::Request::ToString () const {
    auto request (MethodToString (mMethod) + std::string (" ") + mPath + std::string (" ") + std::string ("HTTP/") + VersionToString (mVersion) + std::string ("\r\n"));
    for (const auto& pair : mHeaders) {
        request += pair.first + std::string (": ") + pair.second + std::string ("\r\n");
    }
    request += std::string ("\r\n");
    request += mBody;
    return request;
}

