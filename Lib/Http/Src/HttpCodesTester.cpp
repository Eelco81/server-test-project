
#include "gtest/gtest.h"

#include "HttpCodes.h"

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
    ASSERT_EQ ("-", VersionToString (Version::UNKNOWN_VERSION));
    ASSERT_EQ ("1.0", VersionToString (Version::V10));
    ASSERT_EQ ("1.1", VersionToString (Version::V11));
}

TEST (HttpCodesTester, StringToVersion) {
    ASSERT_EQ (Version::UNKNOWN_VERSION, StringToVersion ("-"));
    ASSERT_EQ (Version::V10, StringToVersion ("1.0"));
    ASSERT_EQ (Version::V11, StringToVersion ("1.1"));
}