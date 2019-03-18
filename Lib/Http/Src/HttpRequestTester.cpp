
#include "gtest/gtest.h"
#include "HttpRequest.h"

TEST (HttpRequestTester, Constructor) {
    HTTP::Request request;
    ASSERT_EQ (HTTP::Method::UNKNOWN_METHOD, request.mMethod);
    ASSERT_EQ (HTTP::Version::UNKNOWN_VERSION, request.mVersion);
    ASSERT_EQ (1u, request.GetAllHeaders ().size ());
}
