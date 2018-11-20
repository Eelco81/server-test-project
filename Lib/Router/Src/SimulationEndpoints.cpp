
#include "SimulationEndpoints.h"
#include "SimService.h"

#include <json.hpp>
using json = nlohmann::json;

/* ---------------------------------------- */

API::SimEndPoint::SimEndPoint (const std::string& inPath, HTTP::Method inMethod, std::shared_ptr<SIM::IService> inService) :
    HTTP::EndPoint (inPath, inMethod), 
    mService (inService)
{
}

API::SimEndPoint::~SimEndPoint () = default;

/* ---------------------------------------- */

API::SimLoadEndPoint::SimLoadEndPoint (const std::string& inPath, std::shared_ptr<SIM::IService> inService) :
    API::SimEndPoint::SimEndPoint (inPath, HTTP::Method::PUT, inService)
{
}

API::SimLoadEndPoint::~SimLoadEndPoint () = default;

void API::SimLoadEndPoint::Execute (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    
    if (mService->IsRunning ()) {
        outResponse.mCode = HTTP::Code::FORBIDDEN;
        return;
    }
    
    json config;
    
    try {
        config = json::parse (inRequest.mBody);
    }
    catch (...) {
        outResponse.mCode = HTTP::Code::BAD_REQUEST;
        return;
    }
    
    try {
        outResponse.mCode = HTTP::Code::ACCEPTED;
        mService->Load (config);
    }
    catch (std::exception& e) {
        
        json j;
        j["error"] = e.what ();
        
        outResponse.mCode = HTTP::Code::INTERNAL_SERVER_ERROR;
        outResponse.SetBody (j.dump (), "application/json");
    }
}

/* ---------------------------------------- */

API::SimStartEndPoint::SimStartEndPoint (const std::string& inPath, std::shared_ptr<SIM::IService> inService) :
    API::SimEndPoint::SimEndPoint (inPath, HTTP::Method::PUT, inService)
{
}

API::SimStartEndPoint::~SimStartEndPoint () = default;

void API::SimStartEndPoint::Execute (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    
    if (!mService->IsLoaded ()) {
        outResponse.mCode = HTTP::Code::FORBIDDEN;
        return;
    }
    
    if (mService->IsRunning ()) {
        outResponse.mCode = HTTP::Code::NOT_MODIFIED;
        return;
    }
    
    try {
        outResponse.mCode = HTTP::Code::ACCEPTED;
        mService->Start ();
    }
    catch (std::exception& e) {
        
        json j;
        j["error"] = e.what ();
        
        outResponse.mCode = HTTP::Code::INTERNAL_SERVER_ERROR;
        outResponse.SetBody (j.dump (), "application/json");
    }
}

/* ---------------------------------------- */

API::SimStopEndPoint::SimStopEndPoint (const std::string& inPath, std::shared_ptr<SIM::IService> inService) :
    API::SimEndPoint::SimEndPoint (inPath, HTTP::Method::DELETE, inService)
{
}

API::SimStopEndPoint::~SimStopEndPoint () = default;

void API::SimStopEndPoint::Execute (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    
    if (!mService->IsRunning ()) {
        outResponse.mCode = HTTP::Code::NOT_MODIFIED;
        return;
    }
    
    try {
        outResponse.mCode = HTTP::Code::ACCEPTED;
        mService->Stop ();
    }
    catch (std::exception& e) {
        
        json j;
        j["error"] = e.what ();
        
        outResponse.mCode = HTTP::Code::INTERNAL_SERVER_ERROR;
        outResponse.SetBody (j.dump (), "application/json");
    }
}

/* ---------------------------------------- */

API::SimGetValuesEndPoint::SimGetValuesEndPoint (const std::string& inPath, std::shared_ptr<SIM::IService> inService) :
    API::SimEndPoint::SimEndPoint (inPath, HTTP::Method::GET, inService)
{
}

API::SimGetValuesEndPoint::~SimGetValuesEndPoint () = default;

void API::SimGetValuesEndPoint::Execute (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    
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
        
        json j;
        j["error"] = e.what ();
        
        outResponse.mCode = HTTP::Code::INTERNAL_SERVER_ERROR;
        outResponse.SetBody (j.dump (), "application/json");
    }
}

/* ---------------------------------------- */

API::SimGetValueEndPoint::SimGetValueEndPoint (const std::string& inPath, std::shared_ptr<SIM::IService> inService) :
    API::SimEndPoint::SimEndPoint (inPath, HTTP::Method::GET, inService)
{
}

API::SimGetValueEndPoint::~SimGetValueEndPoint () = default;

void API::SimGetValueEndPoint::Execute (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    
    if (!mService->IsLoaded ()) {
        outResponse.mCode = HTTP::Code::FORBIDDEN;
        return;
    }
    
    SIM::Path path;
    try {
        json config = json::parse (inRequest.mBody);
        path = SIM::Path (config["path"].get<std::string>());
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
        
        json j;
        j["error"] = e.what ();
        
        outResponse.mCode = HTTP::Code::INTERNAL_SERVER_ERROR;
        outResponse.SetBody (j.dump (), "application/json");
    }
}


/* ---------------------------------------- */

API::SimSetValueEndPoint::SimSetValueEndPoint (const std::string& inPath, std::shared_ptr<SIM::IService> inService) :
    API::SimEndPoint::SimEndPoint (inPath, HTTP::Method::PUT, inService)
{
}

API::SimSetValueEndPoint::~SimSetValueEndPoint () = default;

void API::SimSetValueEndPoint::Execute (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    
    if (!mService->IsLoaded ()) {
        outResponse.mCode = HTTP::Code::FORBIDDEN;
        return;
    }
    
    SIM::Value value;
    try {
        json config = json::parse (inRequest.mBody);
        value = SIM::Value (SIM::Path (config["path"].get<std::string>()), config["value"].get<double>());
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
        
        json j;
        j["error"] = e.what ();
        
        outResponse.mCode = HTTP::Code::INTERNAL_SERVER_ERROR;
        outResponse.SetBody (j.dump (), "application/json");
    }
}