
#include "gmock/gmock.h"
#include "HttpResponseEncoder.h"

namespace {

std::string Encode (HTTP::Response& inResponse) {
    
    HTTP::ResponseEncoder encoder;
    std::string result;
    
    encoder.Pipe ([&](const std::string& data) { result = data; }); 
    encoder.Write (inResponse);
    
    return result;
}

}

using TestParam = std::tuple<std::string, HTTP::Code, HTTP::Version>;
class HttpResponseEncoderTester : public ::testing::TestWithParam<TestParam> {};

INSTANTIATE_TEST_CASE_P (HttpResponseEncoderTester, HttpResponseEncoderTester,
    ::testing::Values(
        std::make_tuple (std::string ("HTTP/1.0 200 OK"), HTTP::Code::OK, HTTP::Version::V10 ),
        std::make_tuple (std::string ("HTTP/1.1 200 OK"), HTTP::Code::OK, HTTP::Version::V11 ),
        std::make_tuple (std::string ("HTTP/1.0 201 Created"), HTTP::CREATED, HTTP::Version::V10 ),
        std::make_tuple (std::string ("HTTP/1.1 201 Created"), HTTP::CREATED, HTTP::Version::V11 ),
        std::make_tuple (std::string ("HTTP/1.0 202 Accepted"), HTTP::ACCEPTED, HTTP::Version::V10 ),
        std::make_tuple (std::string ("HTTP/1.1 202 Accepted"), HTTP::Code::ACCEPTED, HTTP::Version::V11 ),
        std::make_tuple (std::string ("HTTP/1.0 400 Bad Request"), HTTP::Code::BAD_REQUEST, HTTP::Version::V10 ),
        std::make_tuple (std::string ("HTTP/1.1 400 Bad Request"), HTTP::Code::BAD_REQUEST, HTTP::Version::V11 ),
        std::make_tuple (std::string ("HTTP/1.0 403 Forbidden"), HTTP::Code::FORBIDDEN, HTTP::Version::V10 ),
        std::make_tuple (std::string ("HTTP/1.1 403 Forbidden"), HTTP::Code::FORBIDDEN, HTTP::Version::V11 ),
        std::make_tuple (std::string ("HTTP/1.0 404 Not Found"), HTTP::Code::NOT_FOUND, HTTP::Version::V10 ),
        std::make_tuple (std::string ("HTTP/1.1 404 Not Found"), HTTP::Code::NOT_FOUND, HTTP::Version::V11 ),
        std::make_tuple (std::string ("HTTP/1.0 500 Internal Server Error"), HTTP::Code::INTERNAL_SERVER_ERROR, HTTP::Version::V10 ),
        std::make_tuple (std::string ("HTTP/1.1 500 Internal Server Error"), HTTP::Code::INTERNAL_SERVER_ERROR, HTTP::Version::V11 ),
        std::make_tuple (std::string ("HTTP/x.x 0 Unknown"), HTTP::Code::UNKNOWN_CODE, HTTP::Version::UNKNOWN_VERSION )
    )
);

TEST_P (HttpResponseEncoderTester, BuildInitialLine) {
    HTTP::Response response (std::get<1> (GetParam ()), std::get<2> (GetParam ()));
    EXPECT_THAT ( Encode (response), ::testing::HasSubstr (std::get<0> (GetParam ())));
}

TEST_P (HttpResponseEncoderTester, ContainsZeroContentLength) {
    HTTP::Response response (std::get<1> (GetParam ()), std::get<2> (GetParam ()));
    EXPECT_THAT ( Encode (response), ::testing::HasSubstr ("\r\nContent-Length: 0\r\n"));
}

TEST_P (HttpResponseEncoderTester, ContainsLastModified) {
    HTTP::Response response (std::get<1> (GetParam ()), std::get<2> (GetParam ()));
    EXPECT_THAT ( Encode (response), ::testing::HasSubstr ("\r\nLast-Modified: "));
}

TEST_P (HttpResponseEncoderTester, ContainsUserAgent) {
    HTTP::Response response (std::get<1> (GetParam ()), std::get<2> (GetParam ()));
    EXPECT_THAT ( Encode (response), ::testing::HasSubstr ("\r\nUser-Agent: Unknown/0.0.1"));
}
