
#include "gtest/gtest.h"
#include <string>

#include "Xml.h"
#include "Response.h"
#include "Codes.h"

TEST (ResponseTester, PrintingXml) {

    API::Response response;
    response.SetHeader (API::Header ("Path", "Version", API::Codes::kGet));
    response.SetCode (API::Codes::kOk);

    XmlNode node;
    response.ToXml (node);

    std::string error;
    std::string xml;

    ASSERT_TRUE (Xml::Print (node, xml, error, false));
    ASSERT_EQ (std::string ("<Response Code=\"200\"><Header Method=\"GET\" Path=\"Path\" Version=\"Version\" /></Response>"), xml);
}
