
#include <iostream>
#include <json.hpp>
using json = nlohmann::json;

#include "SimExecEndPoint.h"
#include "FileSystem.h"
#include "ApiUtils.h"

API::SIM::ExecEndPoint::ExecEndPoint (const std::string& inPath, std::shared_ptr<::SIM::IService> inService, const std::string& inConfigPath) :
    API::SIM::EndPoint (inPath, inService),
    mConfigPath (inConfigPath)
{
}

API::SIM::ExecEndPoint::~ExecEndPoint () = default;

void API::SIM::ExecEndPoint::Get (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    
    json j;
    j["loaded"] = mService->IsLoaded ();
    j["running"] = mService->IsRunning ();
    
    outResponse.mCode = HTTP::Code::OK;
    outResponse.SetBody (j.dump (), "application/json");
}

void API::SIM::ExecEndPoint::Put (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    
    if (mService->IsLoaded () || mService->IsRunning ()) {
        API::Utils::SetErrorMessage (outResponse, HTTP::Code::FORBIDDEN, "Simulation is already running.");
        return;
    }
    
    std::string id;
    try {
        json body = json::parse (inRequest.mBody);
        id = body["id"].get<std::string> ();
    }
    catch (...) {
        API::Utils::SetErrorMessage (outResponse, HTTP::Code::BAD_REQUEST, "No 'id' in the request body.");
        return;
    }
    
    std::vector<uint8_t> data;
    if (!OS::FileSystem::Read (mConfigPath + id + ".json", data)) {
        API::Utils::SetErrorMessage (outResponse, HTTP::Code::BAD_REQUEST, id + " does not exist.");
        return;
    }
    
    json config;
    try {
        std::string dataStr (reinterpret_cast<const char*>(data.data ()), data.size ());
        config = json::parse (dataStr);
    }
    catch (...) {
        API::Utils::SetErrorMessage (outResponse, HTTP::Code::INTERNAL_SERVER_ERROR, "Config is not in valid json format.");
        return;
    }
    
    try {
        mService->Load (config);
        mService->Start ();
    }
    catch (std::exception& e) {
        API::Utils::SetErrorMessage (outResponse, HTTP::Code::INTERNAL_SERVER_ERROR, e.what ());
        return;
    }
    
    outResponse.mCode = HTTP::Code::ACCEPTED;
}

void API::SIM::ExecEndPoint::Delete (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    
    if (!mService->IsRunning ()) {
        outResponse.mCode = HTTP::Code::NOT_MODIFIED;
        return;
    }
    
    try {
        outResponse.mCode = HTTP::Code::ACCEPTED;
        mService->Stop ();
    }
    catch (std::exception& e) {
        API::Utils::SetErrorMessage (outResponse, HTTP::Code::INTERNAL_SERVER_ERROR, e.what ());
    }
}
