
#include "FileEndPoint.h"
#include "FileSystem.h"

#include <map>

namespace {

std::map<std::string, std::string> contentTypes = {
    { ".html", "text/html" },
    { ".csv", "text/csv" },
    { ".css", "text/css" },
    { ".xml", "text/xml" },
    { ".json", "application/json" },
    { ".js", "application/javascript" }
};

} // end anonymous namespace

API::FileEndPoint::FileEndPoint (const std::string& inPath, const std::string& inFileName) : 
    HTTP::EndPoint (inPath),
    mFileName (inFileName)
{
}

API::FileEndPoint::~FileEndPoint () = default;

void API::FileEndPoint::Get (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
        
    std::vector<uint8_t> data;
    if (OS::FileSystem::Read (mFileName, data)) {
  
        auto type (contentTypes [OS::FileSystem::GetExtension (mFileName)]);
        if (type == "") {
            type = "text/plain";
        }
        
        outResponse.mCode = HTTP::Code::OK;
        outResponse.SetBody (data, type);
    }
    else {
        outResponse.mCode = HTTP::Code::INTERNAL_SERVER_ERROR;
    }
}

void API::FileEndPoint::Put (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    outResponse.mCode = HTTP::Code::NOT_IMPLEMENTED;
}
