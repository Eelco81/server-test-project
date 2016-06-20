
#include "Codes.h"
#include "Response.h"
#include "Request.h"
#include "Buffer.h"
#include "Xml.h"

namespace {
    const std::string kRootTag ("Response");
    const std::string kCodeTag ("Code");
}

API::Response::Response () :
    Message (kRootTag)
{
}

void API::Response::ToXml (XmlNode& outNode) const {
    Message::ToXml (outNode);
    outNode.SetAttribute (kCodeTag, std::to_string (mCode));
}