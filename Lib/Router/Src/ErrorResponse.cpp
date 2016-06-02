
#include "XmlNode.h"
#include "ErrorResponse.h"

namespace {
    const std::string kBodyTag ("Body");
    const std::string kErrorTag ("Error");
    const std::string kMessageTag ("Message");
}

Api::ErrorResponse::ErrorResponse (Codes::Responses inCode, std::string& inErrorMessage) :
    mMessage (inErrorMessage)
{
    SetCode (inCode);
}

Api::ErrorResponse::~ErrorResponse () {
}

void Api::ErrorResponse::ToXml (Core::XmlNode& outNode) const {
    Response::ToXml (outNode);
    Core::XmlNode bodyNode (kBodyTag);
    Core::XmlNode errorNode (kErrorTag);
    errorNode.SetAttribute ("Message", mMessage);
    bodyNode.AddChild (errorNode);
    outNode.AddChild (bodyNode);
}
