
#include "gmock/gmock.h"
#include "HttpResponse.h"

using TestParam = std::tuple<std::string, HTTP::Code, HTTP::Version>;
class HttpResponseTester : public ::testing::TestWithParam<TestParam> {};

TEST_F (HttpResponseTester, Constructor) {
    HTTP::Response response;
    ASSERT_EQ (response.mVersion, HTTP::Version::UNKNOWN_VERSION);
    ASSERT_EQ (response.mCode, HTTP::Code::UNKNOWN_CODE);
}

INSTANTIATE_TEST_CASE_P (HttpResponseTester, HttpResponseTester,
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

TEST_P (HttpResponseTester, BuildInitialLine) {
    HTTP::Response response (std::get<1> (GetParam ()), std::get<2> (GetParam ()));
    EXPECT_THAT ( response.ToString (), ::testing::HasSubstr (std::get<0> (GetParam ())));
}

TEST_P (HttpResponseTester, ContainsZeroContentLength) {
    HTTP::Response response (std::get<1> (GetParam ()), std::get<2> (GetParam ()));
    EXPECT_THAT ( response.ToString (), ::testing::HasSubstr ("\r\nContent-Length: 0\r\n"));
}

TEST_P (HttpResponseTester, ContainsLastModified) {
    HTTP::Response response (std::get<1> (GetParam ()), std::get<2> (GetParam ()));
    EXPECT_THAT ( response.ToString (), ::testing::HasSubstr ("\r\nLast-Modified: "));
}

TEST_P (HttpResponseTester, ContainsUserAgent) {
    HTTP::Response response (std::get<1> (GetParam ()), std::get<2> (GetParam ()));
    EXPECT_THAT ( response.ToString (), ::testing::HasSubstr ("\r\nUser-Agent: HttpServer/0.0.1"));
}

TEST_F (HttpResponseTester, Bodies) {
    {
        HTTP::Response response (HTTP::Code::OK, HTTP::Version::V11);
        response.SetBody ("HELLO");
        ASSERT_EQ (std::string ("HELLO"), response.GetBody ());
        ASSERT_EQ (std::string ("text/plain"), response.mHeaders[HTTP::Header::CONTENT_TYPE]);
        ASSERT_EQ (std::string ("5"), response.mHeaders[HTTP::Header::CONTENT_LENGTH]);
    }
    {
        HTTP::Response response (HTTP::Code::OK, HTTP::Version::V11);
        response.SetBody ("HELLO", "application/json");
        ASSERT_EQ (std::string ("HELLO"), response.GetBody ());
        ASSERT_EQ (std::string ("application/json"), response.mHeaders[HTTP::Header::CONTENT_TYPE]);
        ASSERT_EQ (std::string ("5"), response.mHeaders[HTTP::Header::CONTENT_LENGTH]);
    }
    {
        HTTP::Response response (HTTP::Code::OK, HTTP::Version::V11);
        response.SetBody (std::vector<uint8_t> ({ 0x01, 0x01 }));
        ASSERT_EQ (std::string ("\x1\x1"), response.GetBody ());
        ASSERT_EQ (std::string ("application/octet-stream"), response.mHeaders[HTTP::Header::CONTENT_TYPE]);
        ASSERT_EQ (std::string ("2"), response.mHeaders[HTTP::Header::CONTENT_LENGTH]);
    }
    {
        HTTP::Response response (HTTP::Code::OK, HTTP::Version::V11);
        response.SetBody (std::vector<uint8_t> ({ 0x01, 0x01 }), "application/json");
        ASSERT_EQ (std::string ("\x1\x1"), response.GetBody ());
        ASSERT_EQ (std::string ("application/json"), response.mHeaders[HTTP::Header::CONTENT_TYPE]);
        ASSERT_EQ (std::string ("2"), response.mHeaders[HTTP::Header::CONTENT_LENGTH]);
    }
}