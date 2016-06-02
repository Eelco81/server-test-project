
#include "Message.h"
#include "Buffer.h"
#include "Xml.h"

namespace {
    const std::string kHeaderTag ("Header");
    const std::string kBodyTag ("Body");
}

Api::Message::Message (const std::string& inRootTag) :
    mRootTag (inRootTag)
{
}

Api::Message::Message (const std::string& inRootTag, const Message& inMessage) :
    mRootTag (inRootTag),
    mHeader (inMessage.mHeader)
{
}

Api::Message::~Message () {
}

bool Api::Message::FromXml (const Core::XmlNode& inNode, std::string& outError) {

    if (inNode.GetName () != mRootTag) {
        outError = "Not a valid root tag " + mRootTag;
        return false;
    }

    Core::XmlNode headerNode;
    if (!inNode.FindChild (kHeaderTag, headerNode)) {
        outError = "Not a valid header found";
        return false;
    }

    if (!mHeader.FromXml (headerNode, outError)) {
        return false;
    }

    return true;
}

void Api::Message::ToXml (Core::XmlNode& outNode) const {
    outNode.SetName (mRootTag);
    Core::XmlNode headerNode;
    mHeader.ToXml (headerNode);
    outNode.AddChild (headerNode);
}
