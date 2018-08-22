
#include <json.hpp>

#include "Version.h"
#include "VersionEndPoint.h"

using json = nlohmann::json;

API::VersionEndPoint::VersionEndPoint () : 
    HTTP::EndPoint ("/api/version", HTTP::Method::GET)
{
}

API::VersionEndPoint::~VersionEndPoint () = default;

void API::VersionEndPoint::Execute (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    outResponse.mCode = HTTP::Code::OK;
    
    json j;
    j["application"] = OS::Version::GetApplicationName ();
    j["version"] = OS::Version::GetApplicationVersion ();
    
    outResponse.mBody = j.dump ();
    
    outResponse.mHeaders[HTTP::Header::CONTENT_TYPE] = "application/json";
    outResponse.mHeaders[HTTP::Header::CONTENT_LENGTH] = std::to_string (outResponse.mBody.size ());
}
