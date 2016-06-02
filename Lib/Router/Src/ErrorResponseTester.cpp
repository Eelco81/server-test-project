
#include "gtest/gtest.h"
#include <string>

#include "Xml.h"
#include "ErrorResponse.h"

TEST (ErrorResponseTester, PrintingXml) {

    Api::ErrorResponse response (Api::Codes::kInternalServerError, std::string ("Something bad happened"));
    response.SetHeader (Api::Header ("Path", "Version", Api::Codes::kGet));

    Core::XmlNode node;
    response.ToXml (node);

    std::string error;
    std::string xml;

    ASSERT_TRUE (Core::Xml::Print (node, xml, error, false));
    ASSERT_EQ (std::string ("<Response Code=\"500\"><Header Method=\"GET\" Path=\"Path\" Version=\"Version\" /><Body><Error Message=\"Something bad happened\" /></Body></Response>"), xml);
}
