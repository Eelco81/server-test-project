
#include <json.hpp>

#include "Version.h"
#include "VersionEndPoint.h"

using json = nlohmann::json;

API::VersionEndPoint::VersionEndPoint (const std::string& inPath) : 
    HTTP::EndPoint (inPath, HTTP::Method::GET)
{
}

API::VersionEndPoint::~VersionEndPoint () = default;

void API::VersionEndPoint::Execute (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    
    json j;
    j["application"] = OS::Version::GetApplicationName ();
    j["version"] = OS::Version::GetApplicationVersion ();
    
    outResponse.mCode = HTTP::Code::OK;
    outResponse.SetBody (j.dump (), "application/json");
}
