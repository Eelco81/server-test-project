
#include "gtest/gtest.h"
#include <string>

#include "Xml.h"
#include "Response.h"
#include "Codes.h"

TEST (ResponseTester, PrintingXml) {

    Api::Response response;
    response.SetHeader (Api::Header ("Path", "Version", Api::Codes::kGet));
    response.SetCode (Api::Codes::kOk);

    Core::XmlNode node;
    response.ToXml (node);

    std::string error;
    std::string xml;

    ASSERT_TRUE (Core::Xml::Print (node, xml, error, false));
    ASSERT_EQ (std::string ("<Response Code=\"200\"><Header Method=\"GET\" Path=\"Path\" Version=\"Version\" /></Response>"), xml);
}
