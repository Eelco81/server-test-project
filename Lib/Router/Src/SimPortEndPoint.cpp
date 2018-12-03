
#include <iostream>
#include "SimPortEndPoint.h"
#include "SimPath.h"
#include "SimValue.h"
#include "ApiUtils.h"

API::SIM::PortEndPoint::PortEndPoint (const std::string& inPath, std::shared_ptr<::SIM::IService> inService) :
    API::SIM::EndPoint (inPath + "/([A-Za-z0-9\\.]+)", inService)
{
}

API::SIM::PortEndPoint::~PortEndPoint () = default;

void API::SIM::PortEndPoint::Get (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    
    if (!mService->IsLoaded ()) {
        API::Utils::SetErrorMessage (outResponse, HTTP::Code::FORBIDDEN, "No simulation loaded");
        return;
    }
    
    ::SIM::Path path;
    try {
        path = ::SIM::Path (GetParameterList ()[1]);
    }
    catch (...) {
        API::Utils::SetErrorMessage (outResponse, HTTP::Code::BAD_REQUEST, "Not a valid path");
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
        API::Utils::SetErrorMessage (outResponse, HTTP::Code::INTERNAL_SERVER_ERROR, e.what ());
    }
}

void API::SIM::PortEndPoint::Put (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    
    if (!mService->IsLoaded ()) {
        API::Utils::SetErrorMessage (outResponse, HTTP::Code::FORBIDDEN, "No simulation loaded");
        return;
    }
    
    json config;
    try {
        config = json::parse (inRequest.mBody);
    }
    catch (...) {
        API::Utils::SetErrorMessage (outResponse, HTTP::Code::BAD_REQUEST, "Request is not valid json");
        return;
    }
    
    ::SIM::Value value;
    try {
       value = ::SIM::Value (::SIM::Path (GetParameterList()[1]), config["value"].get<double>());
    }
    catch (std::exception& e) {
        API::Utils::SetErrorMessage (outResponse, HTTP::Code::BAD_REQUEST, e.what ());
        return;
    }
    
    try {
        mService->SetValue (value);
        outResponse.mCode = HTTP::Code::OK;
    }
    catch (std::exception& e) {
        API::Utils::SetErrorMessage (outResponse, HTTP::Code::INTERNAL_SERVER_ERROR, e.what ());
    }
}
