

#include "gmock/gmock.h"
#include "HttpRequestEncoder.h"

namespace {

std::string Encode (HTTP::Request& inRequest) {
    
    HTTP::RequestEncoder encoder;
    std::string result;
    
    encoder.Pipe ([&](const std::string& data) { result = data; }); 
    encoder.Write (inRequest);
    
    return result;
}

}

using TestParam = std::tuple<std::string, HTTP::Method, HTTP::Version, std::string>;
class HttpRequestEncoderTester : public ::testing::TestWithParam<TestParam> {};

INSTANTIATE_TEST_CASE_P (HttpRequestEncoderTester, HttpRequestEncoderTester,
    ::testing::Values(
        std::make_tuple (std::string ("GET /some/path HTTP/1.0"), HTTP::GET, HTTP::Version::V10, std::string ("/some/path") ),
        std::make_tuple (std::string ("GET /some/path HTTP/1.1"), HTTP::GET, HTTP::Version::V11, std::string ("/some/path") ),
        std::make_tuple (std::string ("POST /some/path HTTP/1.0"), HTTP::POST, HTTP::Version::V10, std::string ("/some/path") ),
        std::make_tuple (std::string ("POST /some/path HTTP/1.1"), HTTP::POST, HTTP::Version::V11, std::string ("/some/path") ),
        std::make_tuple (std::string ("PUT /some/path HTTP/1.0"), HTTP::PUT, HTTP::Version::V10, std::string ("/some/path") ),
        std::make_tuple (std::string ("PUT /some/path HTTP/1.1"), HTTP::PUT, HTTP::Version::V11, std::string ("/some/path") ),
        std::make_tuple (std::string ("DELETE /some/path HTTP/1.0"), HTTP::DELETE, HTTP::Version::V10, std::string ("/some/path") ),
        std::make_tuple (std::string ("DELETE /some/path HTTP/1.1"), HTTP::DELETE, HTTP::Version::V11, std::string ("/some/path") ),
        std::make_tuple (std::string ("- /some/path HTTP/x.x"), HTTP::UNKNOWN_METHOD, HTTP::Version::UNKNOWN_VERSION, std::string ("/some/path") )
    )
);

TEST_P (HttpRequestEncoderTester, BuildInitialLine) {
    HTTP::Request request (std::get<1> (GetParam ()), std::get<3> (GetParam ()), std::get<2> (GetParam ()));
    EXPECT_THAT ( Encode (request), ::testing::HasSubstr (std::get<0> (GetParam ())));
}

TEST_P (HttpRequestEncoderTester, ContainsZeroContentLength) {
    HTTP::Request request (std::get<1> (GetParam ()), std::get<3> (GetParam ()), std::get<2> (GetParam ()));
    EXPECT_THAT ( Encode (request), ::testing::HasSubstr ("\r\ncontent-length: 0\r\n"));
}

TEST_P (HttpRequestEncoderTester, ContainsLastModified) {
    HTTP::Request request (std::get<1> (GetParam ()), std::get<3> (GetParam ()), std::get<2> (GetParam ()));
    EXPECT_THAT ( Encode (request), ::testing::HasSubstr ("\r\nlast-modified: "));
}

TEST_P (HttpRequestEncoderTester, ContainsUserAgent) {
    HTTP::Request request (std::get<1> (GetParam ()), std::get<3> (GetParam ()), std::get<2> (GetParam ()));
    EXPECT_THAT ( Encode (request), ::testing::HasSubstr ("\r\nuser-agent: Unknown/0.0.1"));
}
