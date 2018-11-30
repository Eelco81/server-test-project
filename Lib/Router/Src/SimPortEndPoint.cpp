
#include "SimPortEndPoint.h"
#include "SimPath.h"
#include "SimValue.h"

API::SIM::PortEndPoint::PortEndPoint (const std::string& inPath, std::shared_ptr<::SIM::IService> inService) :
    API::SIM::EndPoint (inPath + "/([A-Za-z0-9\\.]+)", inService)
{
}

API::SIM::PortEndPoint::~PortEndPoint () = default;

void API::SIM::PortEndPoint::Get (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    
    if (!mService->IsLoaded ()) {
        outResponse.mCode = HTTP::Code::FORBIDDEN;
        return;
    }
    
    ::SIM::Path path;
    try {
        path = ::SIM::Path (GetParameterList ()[1]);
    }
    catch (...) {
        outResponse.mCode = HTTP::Code::BAD_REQUEST;
        return;
    }
    
    try {
        const auto value (mService->GetValue (path));
        
        json j;
        j["path"] = value.mPath.ToString();
        j["value"] = value.mValue;
    
        outResponse.mCode = HTTP::Code::OK;
        outResponse.SetBody (j.dump (), "application/json");
    }
    catch (std::exception& e) {
        SetErrorMessage (outResponse, HTTP::Code::INTERNAL_SERVER_ERROR, e.what ());
    }
}

void API::SIM::PortEndPoint::Put (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    
    if (!mService->IsLoaded ()) {
        outResponse.mCode = HTTP::Code::FORBIDDEN;
        return;
    }
    
    ::SIM::Value value;
    try {
        json config = json::parse (inRequest.mBody);
        value = ::SIM::Value (::SIM::Path (GetParameterList()[1]), config["value"].get<double>());
    }
    catch (...) {
        outResponse.mCode = HTTP::Code::BAD_REQUEST;
        return;
    }
    
    try {
        mService->SetValue (value);
        outResponse.mCode = HTTP::Code::OK;
    }
    catch (std::exception& e) {
        SetErrorMessage (outResponse, HTTP::Code::INTERNAL_SERVER_ERROR, e.what ());
    }
}
