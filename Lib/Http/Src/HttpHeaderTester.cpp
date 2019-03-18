
#include "gmock/gmock.h"
#include "HttpHeader.h"

TEST (HttpHeaderTester, Constructor) {
    HTTP::Header header ("my-key", "my-value");
    EXPECT_EQ (std::string ("my-key"), header.GetKey ());
    EXPECT_EQ (std::string ("my-value"), header.GetValue ());
}

TEST (HttpHeaderTester, GetSetKey) {
    HTTP::Header header;
    header.SetKey ("my-key");
    EXPECT_EQ (std::string ("my-key"), header.GetKey ());
}

TEST (HttpHeaderTester, KeysAreAlwaysLowerCase) {
    {
        HTTP::Header header ("My-KeY-1234", "my-value");
        EXPECT_EQ (std::string ("my-key-1234"), header.GetKey ());
    }
    {
        HTTP::Header header;
        header.SetKey ("My-KeY-1234");
        EXPECT_EQ (std::string ("my-key-1234"), header.GetKey ());
    }
}

TEST (HttpHeaderTester, GetSetValue) {
    HTTP::Header header;
    header.SetValue ("my-value");
    EXPECT_EQ (std::string ("my-value"), header.GetValue ());
}

