
#include "SimConfigEndPoint.h"
#include "ApiUtils.h"

API::SIM::ConfigEndPoint::ConfigEndPoint (const std::string& inPath, std::shared_ptr<::SIM::IService> inService, const std::string& inConfigPath) :
    API::SIM::EndPoint (inPath, inService),
    mConfigPath (inConfigPath)
{
}

API::SIM::ConfigEndPoint::~ConfigEndPoint () = default;

void API::SIM::ConfigEndPoint::Get (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    API::Utils::SetFileMessage (outResponse, mConfigPath + GetParameterList()[1] + ".json");
}
