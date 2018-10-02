
#include "gmock/gmock.h"
#include "HttpResponse.h"

TEST (HttpResponseTester, Constructor) {
    HTTP::Response response;
    ASSERT_EQ (response.mVersion, HTTP::Version::UNKNOWN_VERSION);
    ASSERT_EQ (response.mCode, HTTP::Code::UNKNOWN_CODE);
}

TEST (HttpResponseTester, Bodies) {
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