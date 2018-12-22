
#include "SimSamplersEndPoint.h"
#include "ApiUtils.h"

API::SIM::SamplersEndPoint::SamplersEndPoint (const std::string& inPath, std::shared_ptr<::SIM::IService> inService) :
    API::SIM::EndPoint (inPath, inService)
{
}

API::SIM::SamplersEndPoint::~SamplersEndPoint () = default;

void API::SIM::SamplersEndPoint::Get (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    
    if (!mService->IsLoaded ()) {
        API::Utils::SetErrorMessage (outResponse, HTTP::Code::FORBIDDEN, "No simulation loaded");
        return;
    }
    
    try {
        
        const auto samplers (mService->GetSamplers ());
        
        std::vector<json> output (samplers.size ());
        std::transform (samplers.begin (), samplers.end (), output.begin (), [](const auto& sampler) { 
            
            std::vector<json> ports (sampler.mPortInfos.size ());
        
            std::transform (sampler.mPortInfos.begin (), sampler.mPortInfos.end (), ports.begin (), [](const auto& info) {
                
                json j;
                j["path"] = info.mPath.ToString ();
                j["type"] = info.mType;
                
                return j;
            });
            
            json j;
            j["ports"] = ports;
            j["id"] = sampler.mId;;
            
            return j; 
        });
        
        json j;
        j["samplers"] = output;
        
        outResponse.mCode = HTTP::Code::OK;
        outResponse.SetBody (j.dump (), "application/json");
    }
    catch (std::exception& e) {
        API::Utils::SetErrorMessage (outResponse, HTTP::Code::INTERNAL_SERVER_ERROR, e.what ());
    }
}
