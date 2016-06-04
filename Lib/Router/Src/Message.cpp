
#include "Message.h"
#include "Buffer.h"
#include "Xml.h"

namespace {
    const std::string kHeaderTag ("Header");
    const std::string kBodyTag ("Body");
}

API::Message::Message (const std::string& inRootTag) :
    mRootTag (inRootTag)
{
}

API::Message::Message (const std::string& inRootTag, const Message& inMessage) :
    mRootTag (inRootTag),
    mHeader (inMessage.mHeader)
{
}

API::Message::~Message () {
}

bool API::Message::FromXml (const XmlNode& inNode, std::string& outError) {

    if (inNode.GetName () != mRootTag) {
        outError = "Not a valid root tag " + mRootTag;
        return false;
    }

    XmlNode headerNode;
    if (!inNode.FindChild (kHeaderTag, headerNode)) {
        outError = "Not a valid header found";
        return false;
    }

    if (!mHeader.FromXml (headerNode, outError)) {
        return false;
    }

    return true;
}

void API::Message::ToXml (XmlNode& outNode) const {
    outNode.SetName (mRootTag);
    XmlNode headerNode;
    mHeader.ToXml (headerNode);
    outNode.AddChild (headerNode);
}
