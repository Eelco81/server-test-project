
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

API::Response::Response (const Message& inMessage) :
    Message (kRootTag, inMessage)
{
}

API::Response::~Response () {
}

void API::Response::ToXml (XmlNode& outNode) const {
    Message::ToXml (outNode);
    outNode.SetAttribute (kCodeTag, std::to_string (mCode));
}