
#include "SimEndPoint.h"

API::SIM::EndPoint::EndPoint (const std::string& inPath, std::shared_ptr<::SIM::IService> inService) :
    HTTP::RegexEndPoint (std::regex (inPath)), 
    mService (inService)
{
}

API::SIM::EndPoint::~EndPoint () = default;

void API::SIM::EndPoint::SetErrorMessage (HTTP::Response ioResponse, HTTP::Code inCode, const std::string& inMessage) const {
    json j;
    j["error"] = inMessage;
    ioResponse.SetBody (j.dump (), "application/json");
    ioResponse.mCode = inCode;
}
