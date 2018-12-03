
#include "SimEndPoint.h"

API::SIM::EndPoint::EndPoint (const std::string& inPath, std::shared_ptr<::SIM::IService> inService) :
    HTTP::RegexEndPoint (std::regex (inPath)), 
    mService (inService)
{
}

API::SIM::EndPoint::~EndPoint () = default;
