
#include "gmock/gmock.h"
#include "HttpMessage.h"
#include "HttpCodes.h"

TEST (HttpMessageTester, Bodies) {
    {
        HTTP::Message message;
        message.SetBody ("HELLO");
        ASSERT_EQ (std::string ("HELLO"), message.GetBody ());
        ASSERT_EQ (std::string ("text/plain"), message.GetHeaders (HTTP::Header::CONTENT_TYPE)[0].GetValue ());
        ASSERT_EQ (std::string ("5"), message.GetHeaders (HTTP::Header::CONTENT_LENGTH)[0].GetValue ());
    }
    {
        HTTP::Message message;
        message.SetBody ("HELLO", "application/json");
        ASSERT_EQ (std::string ("HELLO"), message.GetBody ());
        ASSERT_EQ (std::string ("application/json"), message.GetHeaders (HTTP::Header::CONTENT_TYPE)[0].GetValue ());
        ASSERT_EQ (std::string ("5"), message.GetHeaders (HTTP::Header::CONTENT_LENGTH)[0].GetValue ());
    }
    {
        HTTP::Message message;
        message.SetBody (std::vector<uint8_t> ({ 0x01, 0x01 }));
        ASSERT_EQ (std::string ("\x1\x1"), message.GetBody ());
        ASSERT_EQ (std::string ("application/octet-stream"), message.GetHeaders (HTTP::Header::CONTENT_TYPE)[0].GetValue ());
        ASSERT_EQ (std::string ("2"), message.GetHeaders (HTTP::Header::CONTENT_LENGTH)[0].GetValue ());
    }
    {
        HTTP::Message message;
        message.SetBody (std::vector<uint8_t> ({ 0x01, 0x01 }), "application/json");
        ASSERT_EQ (std::string ("\x1\x1"), message.GetBody ());
        ASSERT_EQ (std::string ("application/json"), message.GetHeaders (HTTP::Header::CONTENT_TYPE)[0].GetValue ());
        ASSERT_EQ (std::string ("2"), message.GetHeaders (HTTP::Header::CONTENT_LENGTH)[0].GetValue ());
    }
}


TEST (HttpMessageTester, Headers) {
    
    
    
}