
#include "XmlNode.h"
#include "ErrorResponse.h"

namespace {
    const std::string kBodyTag ("Body");
    const std::string kErrorTag ("Error");
    const std::string kMessageTag ("Message");
}

API::ErrorResponse::ErrorResponse (Codes::Responses inCode, const std::string& inErrorMessage) :
    mMessage (inErrorMessage)
{
    SetCode (inCode);
}

void API::ErrorResponse::ToXml (XmlNode& outNode) const {
    Response::ToXml (outNode);
    XmlNode bodyNode (kBodyTag);
    XmlNode errorNode (kErrorTag);
    errorNode.SetAttribute ("Message", mMessage);
    bodyNode.AddChild (errorNode);
    outNode.AddChild (bodyNode);
}
