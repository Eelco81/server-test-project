

#include <json.hpp>

#include "Timing.h"
#include "TimeEndPoint.h"

using json = nlohmann::json;

API::TimeEndPoint::TimeEndPoint () : 
    HTTP::EndPoint ("/api/time", HTTP::Method::GET)
{
}

API::TimeEndPoint::~TimeEndPoint () = default;

void API::TimeEndPoint::Execute (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    
    json j;
    j["value"] = OS::Timing::Now ();
    j["unit"] = "ms";
    
    outResponse.mCode = HTTP::Code::OK;
    outResponse.SetBody (j.dump (), "application/json");
}
