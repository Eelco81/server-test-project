
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
    
    outResponse.mCode = HTTP::Code::ACCEPTED;
    mService->Load (config);
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
    
    outResponse.mCode = HTTP::Code::ACCEPTED;
    mService->Start ();
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
    
    outResponse.mCode = HTTP::Code::ACCEPTED;
    mService->Stop ();
}

/* ---------------------------------------- */

API::SimGetPathsEndPoint::SimGetPathsEndPoint (const std::string& inPath, std::shared_ptr<SIM::IService> inService) :
    API::SimEndPoint::SimEndPoint (inPath, HTTP::Method::GET, inService)
{
}

API::SimGetPathsEndPoint::~SimGetPathsEndPoint () = default;

void API::SimGetPathsEndPoint::Execute (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    
    if (!mService->IsLoaded ()) {
        outResponse.mCode = HTTP::Code::FORBIDDEN;
        return;
    }
    
    const auto paths (mService->GetPaths ());
    std::vector<std::string> stringPaths (paths.size ());
    std::transform (paths.begin (), paths.end (), stringPaths.begin (), [](const auto& path) { return path.ToString (); });
    
    json j;
    j["ports"] = stringPaths;
    
    outResponse.mCode = HTTP::Code::OK;
    outResponse.SetBody (j.dump (), "application/json");
}
