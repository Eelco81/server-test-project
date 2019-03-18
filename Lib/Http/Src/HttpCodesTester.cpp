
#include "gtest/gtest.h"

#include "HttpCodes.h"
#include "HttpHeader.h"

using namespace HTTP;

TEST (HttpCodesTester, MethodToString) {
    ASSERT_EQ ("-", MethodToString (Method::UNKNOWN_METHOD));
    ASSERT_EQ ("GET", MethodToString (Method::GET));
    ASSERT_EQ ("PUT", MethodToString (Method::PUT));
    ASSERT_EQ ("POST", MethodToString (Method::POST));
    ASSERT_EQ ("DELETE", MethodToString (Method::DELETE));
}

TEST (HttpCodesTester, StringToMethod) {
    ASSERT_EQ (Method::UNKNOWN_METHOD, StringToMethod ("-"));
    ASSERT_EQ (Method::GET, StringToMethod ("GET"));
    ASSERT_EQ (Method::PUT, StringToMethod ("PUT"));
    ASSERT_EQ (Method::POST, StringToMethod ("POST"));
    ASSERT_EQ (Method::DELETE, StringToMethod ("DELETE"));
}

TEST (HttpCodesTester, VersionToString) {
    ASSERT_EQ ("x.x", VersionToString (Version::UNKNOWN_VERSION));
    ASSERT_EQ ("1.0", VersionToString (Version::V10));
    ASSERT_EQ ("1.1", VersionToString (Version::V11));
}

TEST (HttpCodesTester, StringToVersion) {
    ASSERT_EQ (Version::UNKNOWN_VERSION, StringToVersion ("-"));
    ASSERT_EQ (Version::V10, StringToVersion ("1.0"));
    ASSERT_EQ (Version::V11, StringToVersion ("1.1"));
}

TEST (HttpCodesTester, CodeToString) {
    ASSERT_EQ (std::string ("Unknown"), CodeToString (Code::UNKNOWN_CODE));
    ASSERT_EQ (std::string ("Switching Protocols"), CodeToString (Code::SWITCHING_PROTOCOLS)); 
    ASSERT_EQ (std::string ("OK"), CodeToString (Code::OK)); 
    ASSERT_EQ (std::string ("Created"), CodeToString (Code::CREATED)); 
    ASSERT_EQ (std::string ("Accepted"), CodeToString (Code::ACCEPTED)); 
    ASSERT_EQ (std::string ("No Content"), CodeToString (Code::NO_CONTENT)); 
    ASSERT_EQ (std::string ("Not Modified"), CodeToString (Code::NOT_MODIFIED));
    ASSERT_EQ (std::string ("Bad Request"), CodeToString (Code::BAD_REQUEST)); 
    ASSERT_EQ (std::string ("Forbidden"), CodeToString (Code::FORBIDDEN));
    ASSERT_EQ (std::string ("Not Found"), CodeToString (Code::NOT_FOUND)); 
    ASSERT_EQ (std::string ("Internal Server Error"), CodeToString (Code::INTERNAL_SERVER_ERROR));
    ASSERT_EQ (std::string ("Not Implemented"), CodeToString (Code::NOT_IMPLEMENTED));
    ASSERT_EQ (std::string ("HTTP Version Not Supported"), CodeToString (Code::HTTP_VERSION_NOT_SUPPORTED));
}

TEST (HttpCodesTester, Header) {
    ASSERT_EQ (std::string ("Content-Length"), Header::CONTENT_LENGTH);
    ASSERT_EQ (std::string ("Content-Type"), Header::CONTENT_TYPE);
    ASSERT_EQ (std::string ("User-Agent"), Header::USER_AGENT);
    ASSERT_EQ (std::string ("Last-Modified"), Header::LAST_MODIFIED);
    ASSERT_EQ (std::string ("Host"), Header::HOST);
}