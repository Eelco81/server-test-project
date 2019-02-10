
#include <tuple>
#include <string>
#include "gtest/gtest.h"
#include "HttpResponse.h"
#include "HttpResponseDecoder.h"

namespace {

class ResponseHarvester : public HTTP::ResponseDecoder {

public: 
    ResponseHarvester () :
        HTTP::ResponseDecoder () 
    {
        Pipe (this, &ResponseHarvester::HandleResponse);
    }
protected:
    virtual void HandleResponse (const HTTP::Response& inResponse) {
        mResponses.push_back (inResponse);
    }
public:
    std::vector<HTTP::Response> mResponses;
};

}

using TestParamSuccess = std::tuple<std::string, HTTP::Code, HTTP::Version>;
class HttpResponseDecoderSuccessTester : public ::testing::TestWithParam<TestParamSuccess> {};

INSTANTIATE_TEST_CASE_P (HttpResponseDecoderSuccessTester, HttpResponseDecoderSuccessTester,
    ::testing::Values(
        std::make_tuple (std::string ("HTTP/1.0 200 Some Text\r\n\r\n"), HTTP::OK, HTTP::Version::V10 ),
        std::make_tuple (std::string ("HTTP/1.1 200 Some Text\r\n\r\n"), HTTP::OK, HTTP::Version::V11 ),
        std::make_tuple (std::string ("HTTP/1.0 404 Some Text\r\n\r\n"), HTTP::NOT_FOUND, HTTP::Version::V10 ),
        std::make_tuple (std::string ("HTTP/1.1 404 Some Text\r\n\r\n"), HTTP::NOT_FOUND, HTTP::Version::V11 )
        //std::make_tuple (std::string ("HTTP/2.0 12345\r\n\r\n"), HTTP::UNKNOWN_CODE, HTTP::Version::UNKNOWN_VERSION )
    )
);

TEST_P (HttpResponseDecoderSuccessTester, ParseInitialLine) {
    ResponseHarvester decoder;
    decoder.Write (std::get<0> (GetParam ()));
    ASSERT_EQ (std::get<1> (GetParam ()), decoder.mResponses[0].mCode);
    ASSERT_EQ (std::get<2> (GetParam ()), decoder.mResponses[0].mVersion);
    ASSERT_EQ (1u, decoder.mResponses.size ());
    ASSERT_EQ (1u, decoder.mResponses[0].mHeaders.size ());
    ASSERT_EQ (std::string (""), decoder.mResponses[0].GetBody ());
    ASSERT_TRUE (decoder.mResponses[0].mIsValid);
}

TEST_P (HttpResponseDecoderSuccessTester, ParseMultipleMessages) {
    ResponseHarvester decoder;
    const auto message (std::get<0> (GetParam ()));
    decoder.Write (message + message + message + message + message);
    ASSERT_EQ (5u, decoder.mResponses.size());
    for (auto request : decoder.mResponses) {
        ASSERT_EQ (std::get<1> (GetParam ()), request.mCode);
        ASSERT_EQ (std::get<2> (GetParam ()), request.mVersion);
        ASSERT_EQ (1u, request.mHeaders.size ());
        ASSERT_EQ (std::string (""), request.GetBody ());
        ASSERT_TRUE (request.mIsValid);
    }
}

class HttpResponseDecoderErrorTester : public ::testing::TestWithParam<std::string> {};

INSTANTIATE_TEST_CASE_P (HttpResponseDecoderErrorTester, HttpResponseDecoderErrorTester,
    ::testing::Values(
        std::string ("HTTP/1.0 not a code b\r\n"),
        std::string ("HTTP 1.0 400\r\n\r\n"),
        std::string ("HTTP/1.0 \r\n\r\n"),
        std::string ("1.0 400\r\n\r\n"),
        std::string ("HTP/1.0 400\r\n\r\n"),
        std::string ("HTTP/1.1\r\n\r\n"),
        std::string ("\r\n\r\n\r\n\r\n"),
        std::string ("")
    )
);

TEST_P (HttpResponseDecoderErrorTester, InvalidInitialLines) {
    ResponseHarvester decoder;
    decoder.Write (GetParam ());
    ASSERT_EQ (0u, decoder.mResponses.size ());
}

TEST (HttpResponseDecoderTester, Headers){
    {
        ResponseHarvester decoder;
        decoder.Write ("HTTP/1.0 400 Some Text\r\nHeader-Name: Header-Value\r\n\r\n" );
        ASSERT_EQ (1u, decoder.mResponses.size ());
        ASSERT_EQ (std::string ("Header-Value"), decoder.mResponses[0].mHeaders["Header-Name"]);
    }
    {
        ResponseHarvester decoder;
        decoder.Write ("HTTP/1.0 400 Some Text\r\nHeader-Name: Header-Value\r\nHeader-Name-123: Header-Value-123\r\n\r\n" );
        ASSERT_EQ (1u, decoder.mResponses.size ());
        ASSERT_EQ (std::string ("Header-Value"), decoder.mResponses[0].mHeaders["Header-Name"]);
        ASSERT_EQ (std::string ("Header-Value-123"), decoder.mResponses[0].mHeaders["Header-Name-123"]);
    }
    //todo: parametrize this test, add failure cases.
}

TEST (HttpResponseDecoderTester, Bodies){
    {
        ResponseHarvester decoder;
        decoder.Write ("HTTP/1.0 400 Some Text\r\nContent-Length: 20\r\n\r\n01234567890123456789");
        ASSERT_EQ (1u, decoder.mResponses.size ());
        ASSERT_EQ (std::string ("01234567890123456789"), decoder.mResponses[0].GetBody ());
    }
    {
        ResponseHarvester decoder;
        decoder.Write ("HTTP/1.0 400 Some Text\r\nContent-Length: 22\r\n\r\n0123456789\r\n0123456789");
        ASSERT_EQ (1u, decoder.mResponses.size ());
        ASSERT_EQ (std::string ("0123456789\r\n0123456789"), decoder.mResponses[0].GetBody ());
    }
    {
        ResponseHarvester decoder;
        decoder.Write ("HTTP/1.0 400 Some Text\r\nContent-Length: 20\r\n\r\n0123456789");
        decoder.Write ("9876543210");
        ASSERT_EQ (1u, decoder.mResponses.size ());
        ASSERT_EQ (std::string ("01234567899876543210"), decoder.mResponses[0].GetBody ());
    }
    {
        ResponseHarvester decoder;
        decoder.Write ("HTTP/1.0 400 Some Text\r\nContent-Length: 0\r\n\r\n");
        ASSERT_EQ (1u, decoder.mResponses.size ());
        ASSERT_EQ (std::string (""), decoder.mResponses[0].GetBody ());
    }
    {
        ResponseHarvester decoder;
        decoder.Write ("HTTP/1.0 400 Some Text\r\nContent-Length: ABC\r\n\r\n");
        ASSERT_EQ (std::string (""), decoder.mResponses[0].GetBody ());
        ASSERT_EQ (1u, decoder.mResponses.size ());
        ASSERT_FALSE (decoder.mResponses[0].mIsValid);
    }
    // \todo: parametrize this test, add failure cases.
}

//todo: handle multiple messages per test
//todo: process requests that span over multiple messages
