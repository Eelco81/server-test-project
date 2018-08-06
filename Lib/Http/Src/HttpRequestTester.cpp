#include <tuple>
#include <string>
#include "gtest/gtest.h"
#include "HttpRequest.h"

namespace {

class RequestHarvester : public HTTP::RequestParser {
protected:
    virtual void HandleRequest (const HTTP::Request& inRequest) override {
        mRequests.push_back (inRequest);
    }
public:
    std::vector<HTTP::Request> mRequests;
};

}

TEST (HttpRequestTester, Constructor) {
    HTTP::Request request;
    ASSERT_EQ (HTTP::Method::UNKNOWN_METHOD, request.mMethod);
    ASSERT_EQ (HTTP::Version::UNKNOWN_VERSION, request.mVersion);
    ASSERT_EQ (0u, request.mHeaders.size ());
}

using TestParamSuccess = std::tuple<std::string, HTTP::Method, HTTP::Version>;
class HttpRequestParserSuccessTester : public ::testing::TestWithParam<TestParamSuccess> {};

INSTANTIATE_TEST_CASE_P (HttpRequestParserSuccessTester, HttpRequestParserSuccessTester,
    ::testing::Values(
        std::make_tuple (std::string ("GET /some/path HTTP/1.0\n\n"), HTTP::Method::GET, HTTP::Version::V10 ),
        std::make_tuple (std::string ("GET /some/path HTTP/1.1\n\n"), HTTP::Method::GET, HTTP::Version::V11 ),
        std::make_tuple (std::string ("PUT /some/path HTTP/1.0\n\n"), HTTP::Method::PUT, HTTP::Version::V10 ),
        std::make_tuple (std::string ("PUT /some/path HTTP/1.1\n\n"), HTTP::Method::PUT, HTTP::Version::V11 ),
        std::make_tuple (std::string ("POST /some/path HTTP/1.0\n\n"), HTTP::Method::POST, HTTP::Version::V10 ),
        std::make_tuple (std::string ("POST /some/path HTTP/1.1\n\n"), HTTP::Method::POST, HTTP::Version::V11 ),
        std::make_tuple (std::string ("DELETE /some/path HTTP/1.0\n\n"), HTTP::Method::DELETE, HTTP::Version::V10 ),
        std::make_tuple (std::string ("DELETE /some/path HTTP/1.1\n\n"), HTTP::Method::DELETE, HTTP::Version::V11 ),
        std::make_tuple (std::string ("DELETE /some/path HTTP/2.0\n\n"), HTTP::Method::DELETE, HTTP::Version::UNKNOWN_VERSION ),
        std::make_tuple (std::string ("OTHER /some/path HTTP/1.1\n\n"), HTTP::Method::UNKNOWN_METHOD, HTTP::Version::V11 )
    )
);

TEST_P (HttpRequestParserSuccessTester, ParseInitialLine) {
    RequestHarvester parser;
    parser.Write (std::get<0> (GetParam ()));
    ASSERT_EQ (std::get<1> (GetParam ()), parser.mRequests[0].mMethod);
    ASSERT_EQ (std::get<2> (GetParam ()), parser.mRequests[0].mVersion);
    ASSERT_EQ (std::string ("/some/path"), parser.mRequests[0].mPath);
    ASSERT_EQ (0u, parser.mRequests[0].mHeaders.size ());
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
    RequestHarvester parser;
    parser.Write (GetParam ());
    ASSERT_EQ (0u, parser.mRequests.size ());
}
