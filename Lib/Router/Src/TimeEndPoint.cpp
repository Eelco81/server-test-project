
#include "Timing.h"
#include "TimeEndPoint.h"

#include <json.hpp>
using json = nlohmann::json;

API::TimeEndPoint::TimeEndPoint (const std::string& inPath) : 
    HTTP::EndPoint (inPath, HTTP::Method::GET)
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
