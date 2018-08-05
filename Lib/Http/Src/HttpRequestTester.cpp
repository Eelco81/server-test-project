#include <tuple>
#include <string>
#include "gtest/gtest.h"
#include "HttpRequest.h"


TEST (HttpRequestTester, Constructor) {
    HTTP::Request request;
    ASSERT_EQ (HTTP::Method::UNKNOWN_METHOD, request.GetMethod ());
    ASSERT_EQ (HTTP::Version::UNKNOWN_VERSION, request.GetVersion ());
    ASSERT_EQ (0u, request.GetHeaders ().size ());
}

using TestParamSuccess = std::tuple<std::string, HTTP::Method, HTTP::Version>;
class HttpRequestParserSuccessTester : public ::testing::TestWithParam<TestParamSuccess> {};

INSTANTIATE_TEST_CASE_P (HttpRequestParserSuccessTester, HttpRequestParserSuccessTester,
    ::testing::Values(
        std::make_tuple (std::string ("GET /some/path HTTP/1.0"), HTTP::Method::GET, HTTP::Version::V10 ),
        std::make_tuple (std::string ("GET /some/path HTTP/1.1"), HTTP::Method::GET, HTTP::Version::V11 ),
        std::make_tuple (std::string ("PUT /some/path HTTP/1.0"), HTTP::Method::PUT, HTTP::Version::V10 ),
        std::make_tuple (std::string ("PUT /some/path HTTP/1.1"), HTTP::Method::PUT, HTTP::Version::V11 ),
        std::make_tuple (std::string ("POST /some/path HTTP/1.0"), HTTP::Method::POST, HTTP::Version::V10 ),
        std::make_tuple (std::string ("POST /some/path HTTP/1.1"), HTTP::Method::POST, HTTP::Version::V11 ),
        std::make_tuple (std::string ("DELETE /some/path HTTP/1.0"), HTTP::Method::DELETE, HTTP::Version::V10 ),
        std::make_tuple (std::string ("DELETE /some/path HTTP/1.1"), HTTP::Method::DELETE, HTTP::Version::V11 ),
        std::make_tuple (std::string ("DELETE /some/path HTTP/2.0"), HTTP::Method::DELETE, HTTP::Version::UNKNOWN_VERSION ),
        std::make_tuple (std::string ("OTHER /some/path HTTP/1.1"), HTTP::Method::UNKNOWN_METHOD, HTTP::Version::V11 )
    )
);

TEST_P (HttpRequestParserSuccessTester, ParseInitialLine) {
    HTTP::Request request;
    ASSERT_TRUE (request.Parse (std::get<0> (GetParam ())));
    ASSERT_EQ (std::get<1> (GetParam ()), request.GetMethod ());
    ASSERT_EQ (std::get<2> (GetParam ()), request.GetVersion ());
}

class HttpRequestParserErrorTester : public ::testing::TestWithParam<std::string> {};

INSTANTIATE_TEST_CASE_P (HttpRequestParserErrorTester, HttpRequestParserErrorTester,
    ::testing::Values(
        std::string ("GET /some/path HTTP 1.0"),
        std::string ("1234 /some/path HTTP/1.0"),
        std::string ("/some/path HTTP/1.0"),
        std::string ("GET /some/path 1.0"),
        std::string ("GET /some/path HTP/1.0"),
        std::string ("")
    )
);

TEST_P (HttpRequestParserErrorTester, InvalidInitialLines) {
    HTTP::Request request;
    ASSERT_FALSE (request.Parse (GetParam ()));
}
