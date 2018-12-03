
#include "DirEndPoint.h"
#include "ApiUtils.h"

API::DirEndPoint::DirEndPoint (const std::string& inPath, const std::string& inDirName, const std::string& inExtension) : 
    HTTP::RegexEndPoint (std::regex (inPath + "/([\\.A-Za-z0-9]+)")),
    mDirName (inDirName),
    mExtension (inExtension)
{
}

API::DirEndPoint::~DirEndPoint () = default;

void API::DirEndPoint::Get (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    API::Utils::SetFileMessage (outResponse, mDirName + "/" + GetParameterList()[1] + mExtension);
}

void API::DirEndPoint::Put (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    API::Utils::SetErrorMessage (outResponse, HTTP::Code::NOT_IMPLEMENTED, "Not available yet");
}
