
#include <json.hpp>
using json = nlohmann::json;

#include "ListDirEndPoint.h"
#include "ApiUtils.h"
#include "FileSystem.h"

API::ListDirEndPoint::ListDirEndPoint (const std::string& inPath, const std::string& inDirName, bool inIncludeExtensions) : 
    HTTP::EndPoint (inPath),
    mDirName (inDirName),
    mIncludeExtensions (inIncludeExtensions)
{
}

API::ListDirEndPoint::~ListDirEndPoint () = default;

void API::ListDirEndPoint::Get (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    
    std::vector<std::string> list;
    if (!OS::FileSystem::ListDir (mDirName, list, mIncludeExtensions)) {
        API::Utils::SetErrorMessage (outResponse, HTTP::Code::INTERNAL_SERVER_ERROR, "Failed to read resources");
        return;
    }
    
    json j;
    j = list;
    
    outResponse.SetBody (j.dump (), "application/json");
    outResponse.mCode = HTTP::Code::OK;
}
