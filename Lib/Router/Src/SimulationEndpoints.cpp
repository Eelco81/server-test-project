
#include "SimulationEndpoints.h"
#include "SimService.h"

#include <json.hpp>
using json = nlohmann::json;

/* ---------------------------------------- */

API::SimEndPoint::SimEndPoint (const std::string& inPath, HTTP::Method inMethod, std::shared_ptr<SIM::Service> inService) :
    HTTP::EndPoint (inPath, inMethod), 
    mService (inService)
{
}

API::SimEndPoint::~SimEndPoint () = default;

/* ---------------------------------------- */

API::SimLoadEndPoint::SimLoadEndPoint (std::shared_ptr<SIM::Service> inService) :
    API::SimEndPoint::SimEndPoint ("/api/sim", HTTP::Method::PUT, inService)
{
}

API::SimLoadEndPoint::~SimLoadEndPoint () = default;

void API::SimLoadEndPoint::Execute (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    
    auto config = json::parse (R"({
            "step": 100,
            "blocks" : [{
                "name" : "MyName",
                "type" : "MyType"
            }]
        })");
    
    mService->Load (config);
}

/* ---------------------------------------- */

API::SimStartEndPoint::SimStartEndPoint (std::shared_ptr<SIM::Service> inService) :
    API::SimEndPoint::SimEndPoint ("/api/sim", HTTP::Method::POST, inService)
{
}

API::SimStartEndPoint::~SimStartEndPoint () = default;

void API::SimStartEndPoint::Execute (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    mService->Start ();
}

/* ---------------------------------------- */

API::SimStopEndPoint::SimStopEndPoint (std::shared_ptr<SIM::Service> inService) :
    API::SimEndPoint::SimEndPoint ("/api/sim", HTTP::Method::DELETE, inService)
{
}

API::SimStopEndPoint::~SimStopEndPoint () = default;

void API::SimStopEndPoint::Execute (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    mService->Stop ();
}