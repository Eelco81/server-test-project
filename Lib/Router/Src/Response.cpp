
#include "Codes.h"
#include "Response.h"
#include "Request.h"
#include "Buffer.h"
#include "Xml.h"

namespace {
    const std::string kRootTag ("Response");
    const std::string kCodeTag ("Code");
}

Api::Response::Response () :
    Message (kRootTag)
{
}

Api::Response::Response (const Message& inMessage) :
    Message (kRootTag, inMessage)
{
}

Api::Response::~Response () {
}

void Api::Response::ToXml (Core::XmlNode& outNode) const {
    Message::ToXml (outNode);
    outNode.SetAttribute (kCodeTag, std::to_string (mCode));
}