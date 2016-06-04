
#include "Header.h"
#include "XmlNode.h"

namespace {
    const std::string kHeaderTag ("Header");
    const std::string kMethodTag ("Method");
    const std::string kPathTag ("Path");
    const std::string kVersionTag ("Version");
}

Api::Header::Header () {
}

Api::Header::Header (const std::string& inPath, const std::string& inVersion, Codes::Methods inMethod) :
    mPath (inPath),
    mVersion (inVersion),
    mMethod (inMethod) 
{
}

Api::Header::~Header () {
}

bool Api::Header::FromXml (const Core::XmlNode& inNode, std::string& outError) {

    if (!inNode.GetAttribute (kPathTag, mPath)) {
        outError = "Not a valid path tag " + kPathTag;
        return false;
    }

    if (!inNode.GetAttribute (kVersionTag, mVersion)) {
        outError = "Not a valid version tag " + kVersionTag;
        return false;
    }

    std::string method;
    if (!inNode.GetAttribute (kMethodTag, method)) {
        outError = "Not a valid method tag " + kMethodTag;
        return false;
    }
    mMethod = Codes::MethodFromString (method);

    return true;
}

void Api::Header::ToXml (Core::XmlNode& outNode) const {

    outNode.SetName (kHeaderTag);
    outNode.SetAttribute (kPathTag, mPath);
    outNode.SetAttribute (kVersionTag, mVersion);
    outNode.SetAttribute (kMethodTag, Codes::MethodToString (mMethod));
}