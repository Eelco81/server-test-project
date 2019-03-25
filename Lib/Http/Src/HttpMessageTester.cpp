
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

    HTTP::Message message;
    ASSERT_EQ (1u, message.GetAllHeaders ().size ());
    ASSERT_EQ (std::string ("0"), message.GetHeaders (HTTP::Header::CONTENT_LENGTH)[0].GetValue ());
    
    message.SetHeader (HTTP::Header ("Key", "Value"));
    ASSERT_EQ (2u, message.GetAllHeaders ().size ());
    ASSERT_EQ (1u, message.GetHeaders ("key").size ());
    ASSERT_EQ (std::string ("Value"), message.GetHeaderValue ("Key"));
    
    ASSERT_EQ (0u, message.GetHeaders ("Different-Key").size ());
    ASSERT_EQ ("", message.GetHeaderValue("Different-Key"));
    
    message.SetHeader (HTTP::Header ("Key", "New-Value"));
    ASSERT_EQ (1u, message.GetHeaders ("key").size ());
    ASSERT_EQ (std::string ("New-Value"), message.GetHeaderValue ("Key"));
    
    message.AddHeader (HTTP::Header ("Key", "Added-Value"));
    ASSERT_EQ (std::string ("New-Value"), message.GetHeaderValue ("Key"));
    ASSERT_EQ (2u, message.GetHeaders ("key").size ());
    ASSERT_EQ (std::string ("New-Value") , message.GetHeaders ("key")[0].GetValue ());
    ASSERT_EQ (std::string ("Added-Value"), message.GetHeaders ("key")[1].GetValue ());
}
