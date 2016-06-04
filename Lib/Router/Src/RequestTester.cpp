
#include "gtest/gtest.h"
#include <string>

#include "Xml.h"
#include "Request.h"

TEST (RequestTester, ParsingXml) {

    std::string xml ("<Request><Header Path='Path' Version='Version' Method='GET' /></Request>");
    XmlNode node;
    std::string error;
    ASSERT_TRUE (Xml::Parse (xml, node, error));

    API::Request request;
    ASSERT_TRUE (request.FromXml (node, error));

    ASSERT_EQ (std::string ("Version"), request.GetHeader ().mVersion);
    ASSERT_EQ (API::Codes::kGet, request.GetHeader ().mMethod);
    ASSERT_EQ (std::string ("Path"), request.GetHeader ().mPath);
}
/*
TEST (RequestTester, ParsingErrorneousXml) {
    {
        std::string xml ("<Request></Request>");
        Core::XmlNode node;
        std::string error;
        ASSERT_TRUE (Core::Xml::Parse (xml, node, error));
        API::Request request;
        ASSERT_FALSE (request.FromXml (node, error));
        ASSERT_EQ (std::string ("Not a valid header found"), error);
    }
    {
        std::string xml ("<Request><Header/></Request>");
        Core::XmlNode node;
        std::string error;
        ASSERT_TRUE (Core::Xml::Parse (xml, node, error));
        API::Request request;
        ASSERT_FALSE (request.FromXml (node, error));
        ASSERT_EQ (std::string ("Not a valid path tag Path"), error);
    }
    {
        std::string xml ("<Request><Header Path='Path'/></Request>");
        Core::XmlNode node;
        std::string error;
        ASSERT_TRUE (Core::Xml::Parse (xml, node, error));
        API::Request request;
        ASSERT_FALSE (request.FromXml (node, error));
        ASSERT_EQ (std::string ("Not a valid version tag Version"), error);
    }
    {
        std::string xml ("<Request><Header Path='Path' Version='Version'/></Request>");
        Core::XmlNode node;
        std::string error;
        ASSERT_TRUE (Core::Xml::Parse (xml, node, error));
        API::Request request;
        ASSERT_FALSE (request.FromXml (node, error));
        ASSERT_EQ (std::string ("Not a valid method tag Method"), error);
    }
}

*/