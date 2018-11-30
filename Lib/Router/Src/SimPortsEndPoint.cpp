
#include "SimPortsEndPoint.h"

API::SIM::PortsEndPoint::PortsEndPoint (const std::string& inPath, std::shared_ptr<::SIM::IService> inService) :
    API::SIM::EndPoint (inPath, inService)
{
}

API::SIM::PortsEndPoint::~PortsEndPoint () = default;

void API::SIM::PortsEndPoint::Get (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    
    if (!mService->IsLoaded ()) {
        outResponse.mCode = HTTP::Code::FORBIDDEN;
        return;
    }
    
    try {
        const auto values (mService->GetValues ());
        
        std::vector<json> output (values.size ());
        std::transform (values.begin (), values.end (), output.begin (), [](const auto& value) { 
        
            json j;
            j["path"] = value.mPath.ToString();
            j["value"] = value.mValue;
            
            return j; 
        });
        
        json j;
        j["ports"] = output;
        
        outResponse.mCode = HTTP::Code::OK;
        outResponse.SetBody (j.dump (), "application/json");
    }
    catch (std::exception& e) {
        SetErrorMessage (outResponse, HTTP::Code::INTERNAL_SERVER_ERROR, e.what ());
    }
}
