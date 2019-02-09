
#include "gmock/gmock.h"
#include "HttpMessage.h"
#include "HttpCodes.h"

TEST (HttpMessageTester, Bodies) {
    {
        HTTP::Message message;
        message.SetBody ("HELLO");
        ASSERT_EQ (std::string ("HELLO"), message.GetBody ());
        ASSERT_EQ (std::string ("text/plain"), message.mHeaders[HTTP::Header::CONTENT_TYPE]);
        ASSERT_EQ (std::string ("5"), message.mHeaders[HTTP::Header::CONTENT_LENGTH]);
    }
    {
        HTTP::Message message;
        message.SetBody ("HELLO", "application/json");
        ASSERT_EQ (std::string ("HELLO"), message.GetBody ());
        ASSERT_EQ (std::string ("application/json"), message.mHeaders[HTTP::Header::CONTENT_TYPE]);
        ASSERT_EQ (std::string ("5"), message.mHeaders[HTTP::Header::CONTENT_LENGTH]);
    }
    {
        HTTP::Message message;
        message.SetBody (std::vector<uint8_t> ({ 0x01, 0x01 }));
        ASSERT_EQ (std::string ("\x1\x1"), message.GetBody ());
        ASSERT_EQ (std::string ("application/octet-stream"), message.mHeaders[HTTP::Header::CONTENT_TYPE]);
        ASSERT_EQ (std::string ("2"), message.mHeaders[HTTP::Header::CONTENT_LENGTH]);
    }
    {
        HTTP::Message message;
        message.SetBody (std::vector<uint8_t> ({ 0x01, 0x01 }), "application/json");
        ASSERT_EQ (std::string ("\x1\x1"), message.GetBody ());
        ASSERT_EQ (std::string ("application/json"), message.mHeaders[HTTP::Header::CONTENT_TYPE]);
        ASSERT_EQ (std::string ("2"), message.mHeaders[HTTP::Header::CONTENT_LENGTH]);
    }
}