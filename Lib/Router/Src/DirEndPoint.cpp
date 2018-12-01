
#include "DirEndPoint.h"
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

API::DirEndPoint::DirEndPoint (const std::string& inPath, const std::string& inDirName) : 
    HTTP::RegexEndPoint (std::regex (inPath + "/([\\.A-Za-z0-9]+)")),
    mDirName (inDirName)
{
}

API::DirEndPoint::~DirEndPoint () = default;

void API::DirEndPoint::Get (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    
    std::string data;
    if (OS::FileSystem::Read (mDirName + "/" + GetParameterList()[1], data)) {
  
        auto type (contentTypes [OS::FileSystem::GetExtension (GetParameterList()[1])]);
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

void API::DirEndPoint::Put (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
    outResponse.mCode = HTTP::Code::NOT_IMPLEMENTED;
}
