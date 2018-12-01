
#include "SimConfigEndPoint.h"
#include "FileSystem.h"

API::SIM::ConfigEndPoint::ConfigEndPoint (const std::string& inPath, std::shared_ptr<::SIM::IService> inService, const std::string& inConfigPath) :
    API::SIM::EndPoint (inPath, inService),
    mConfigPath (inConfigPath)
{
}

API::SIM::ConfigEndPoint::~ConfigEndPoint () = default;

void API::SIM::ConfigEndPoint::Get (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    
    std::vector<uint8_t> data;
    if (OS::FileSystem::Read (mConfigPath + GetParameterList()[1] + ".json", data)) {
        outResponse.mCode = HTTP::Code::OK;
        outResponse.SetBody (data, "application/json");
    }
    else {
        SetErrorMessage (outResponse, HTTP::Code::BAD_REQUEST, GetParameterList()[1] + " does not exist.");
    }
}
