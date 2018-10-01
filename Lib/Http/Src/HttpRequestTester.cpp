#include <tuple>
#include <string>
#include "gtest/gtest.h"
#include "HttpRequest.h"
#include "HttpRequestDecoder.h"

namespace {

class RequestHarvester : public HTTP::RequestDecoder {

public: 
    RequestHarvester () :
        HTTP::RequestDecoder () 
    {
        Pipe (this, &RequestHarvester::HandleRequest);
    }
protected:
    virtual void HandleRequest (const HTTP::Request& inRequest) {
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
        std::make_tuple (std::string ("GET /some/path HTTP/1.0\r\n\r\n"), HTTP::Method::GET, HTTP::Version::V10 ),
        std::make_tuple (std::string ("GET /some/path HTTP/1.1\r\n\r\n"), HTTP::Method::GET, HTTP::Version::V11 ),
        std::make_tuple (std::string ("PUT /some/path HTTP/1.0\r\n\r\n"), HTTP::Method::PUT, HTTP::Version::V10 ),
        std::make_tuple (std::string ("PUT /some/path HTTP/1.1\r\n\r\n"), HTTP::Method::PUT, HTTP::Version::V11 ),
        std::make_tuple (std::string ("POST /some/path HTTP/1.0\r\n\r\n"), HTTP::Method::POST, HTTP::Version::V10 ),
        std::make_tuple (std::string ("POST /some/path HTTP/1.1\r\n\r\n"), HTTP::Method::POST, HTTP::Version::V11 ),
        std::make_tuple (std::string ("DELETE /some/path HTTP/1.0\r\n\r\n"), HTTP::Method::DELETE, HTTP::Version::V10 ),
        std::make_tuple (std::string ("DELETE /some/path HTTP/1.1\r\n\r\n"), HTTP::Method::DELETE, HTTP::Version::V11 ),
        std::make_tuple (std::string ("DELETE /some/path HTTP/2.0\r\n\r\n"), HTTP::Method::DELETE, HTTP::Version::UNKNOWN_VERSION ),
        std::make_tuple (std::string ("OTHER /some/path HTTP/1.1\r\n\r\n"), HTTP::Method::UNKNOWN_METHOD, HTTP::Version::V11 ),
        std::make_tuple (std::string ("\nBLABLA\nBLABLA\nGET /some/path HTTP/1.0\r\n\r\n"), HTTP::Method::GET, HTTP::Version::V10 )
    )
);

TEST_P (HttpRequestParserSuccessTester, ParseInitialLine) {
    RequestHarvester decoder;
    decoder.Write (std::get<0> (GetParam ()));
    ASSERT_EQ (std::get<1> (GetParam ()), decoder.mRequests[0].mMethod);
    ASSERT_EQ (std::get<2> (GetParam ()), decoder.mRequests[0].mVersion);
    ASSERT_EQ (std::string ("/some/path"), decoder.mRequests[0].mPath);
    ASSERT_EQ (1u, decoder.mRequests.size ());
    ASSERT_EQ (0u, decoder.mRequests[0].mHeaders.size ());
    ASSERT_EQ (std::string (""), decoder.mRequests[0].mBody);
    ASSERT_TRUE (decoder.mRequests[0].mIsValid);
}

TEST_P (HttpRequestParserSuccessTester, ParseMultipleMessages) {
    RequestHarvester decoder;
    const auto message (std::get<0> (GetParam ()));
    decoder.Write (message + message + message + message + message);
    ASSERT_EQ (5u, decoder.mRequests.size());
    for (auto request : decoder.mRequests) {
        ASSERT_EQ (std::get<1> (GetParam ()), request.mMethod);
        ASSERT_EQ (std::get<2> (GetParam ()), request.mVersion);
        ASSERT_EQ (std::string ("/some/path"), request.mPath);
        ASSERT_EQ (0u, request.mHeaders.size ());
        ASSERT_EQ (std::string (""), request.mBody);
        ASSERT_TRUE (request.mIsValid);
    }
}

class HttpRequestParserErrorTester : public ::testing::TestWithParam<std::string> {};

INSTANTIATE_TEST_CASE_P (HttpRequestParserErrorTester, HttpRequestParserErrorTester,
    ::testing::Values(
        std::string ("GET /some/path HTTP/1.0\r\n"),
        std::string ("GET /some/path HTTP 1.0\r\n\r\n"),
        std::string ("1234 /some/path HTTP/1.0\r\n\r\n"),
        std::string ("/some/path HTTP/1.0\r\n\r\n"),
        std::string ("GET /some/path 1.0\r\n\r\n"),
        std::string ("GET /some/path HTP/1.0\r\n\r\n"),
        std::string ("GET HTTP/1.1\r\n\r\n"),
        std::string ("\r\n\r\n\r\n\r\n"),
        std::string ("")
    )
);

TEST_P (HttpRequestParserErrorTester, InvalidInitialLines) {
    RequestHarvester decoder;
    decoder.Write (GetParam ());
    ASSERT_EQ (0u, decoder.mRequests.size ());
}

TEST (HttpRequestParserTester, Headers){
    {
        RequestHarvester decoder;
        decoder.Write ("GET /some/path HTTP/1.0\r\nHeader-Name: Header-Value\r\n\r\n" );
        ASSERT_EQ (1u, decoder.mRequests.size ());
        ASSERT_EQ (std::string ("Header-Value"), decoder.mRequests[0].mHeaders["Header-Name"]);
    }
    {
        RequestHarvester decoder;
        decoder.Write ("GET /some/path HTTP/1.0\r\nHeader-Name: Header-Value\r\nHeader-Name-123: Header-Value-123\r\n\r\n" );
        ASSERT_EQ (1u, decoder.mRequests.size ());
        ASSERT_EQ (std::string ("Header-Value"), decoder.mRequests[0].mHeaders["Header-Name"]);
        ASSERT_EQ (std::string ("Header-Value-123"), decoder.mRequests[0].mHeaders["Header-Name-123"]);
    }
    //todo: parametrize this test, add failure cases.
}

TEST (HttpRequestParserTester, Bodies){
    {
        RequestHarvester decoder;
        decoder.Write ("GET /some/path HTTP/1.0\r\nContent-Length: 20\r\n\r\n01234567890123456789");
        ASSERT_EQ (1u, decoder.mRequests.size ());
        ASSERT_EQ (std::string ("01234567890123456789"), decoder.mRequests[0].mBody);
    }
    {
        RequestHarvester decoder;
        decoder.Write ("GET /some/path HTTP/1.0\r\nContent-Length: 20\r\n\r\n0123456789");
        decoder.Write ("9876543210");
        ASSERT_EQ (1u, decoder.mRequests.size ());
        ASSERT_EQ (std::string ("01234567899876543210"), decoder.mRequests[0].mBody);
    }
    {
        RequestHarvester decoder;
        decoder.Write ("GET /some/path HTTP/1.0\r\nContent-Length: 0\r\n\r\n");
        ASSERT_EQ (1u, decoder.mRequests.size ());
        ASSERT_EQ (std::string (""), decoder.mRequests[0].mBody);
    }
    {
        RequestHarvester decoder;
        decoder.Write ("GET /some/path HTTP/1.0\r\nContent-Length: ABC\r\n\r\n");
        ASSERT_EQ (std::string (""), decoder.mRequests[0].mBody);
        ASSERT_EQ (1u, decoder.mRequests.size ());
        ASSERT_FALSE (decoder.mRequests[0].mIsValid);
    }
    //todo: parametrize this test, add failure cases.
}

//todo: handle multiple messages per test
//todo: process requests that span over multiple messages
