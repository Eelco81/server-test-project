
#include "FileEndPoint.h"
#include "Files.h"

#include <map>

namespace {

std::map<std::string, std::string> contentTypes = {
    { "html", "text/html" },
    { "csv", "text/csv" },
    { "css", "text/css" },
    { "xml", "text/xml" },
    { "json", "application/json" },
    { "js", "application/javascript" }
};

} // end anonymous namespace

API::FileEndPoint::FileEndPoint (const std::string& inPath, const std::string& inFileName) : 
    HTTP::EndPoint (inPath, HTTP::Method::GET),
    mFileName (inFileName)
{
}

API::FileEndPoint::~FileEndPoint () = default;

void API::FileEndPoint::Execute (const HTTP::Request& inRequest, HTTP::Response& outResponse) {
        
    std::vector<uint8_t> data;
    if (OS::Files::Read (mFileName, data)) {
        outResponse.mCode = HTTP::Code::OK;
        outResponse.mBody.assign (reinterpret_cast<char*> (data.data ()), data.size ());
        outResponse.mHeaders[HTTP::Header::CONTENT_LENGTH] = std::to_string (outResponse.mBody.size ());
        
        auto type (contentTypes [OS::Files::GetExtension (mFileName)]);
        if (type == "") {
            type = "text/plain";
        }
        outResponse.mHeaders[HTTP::Header::CONTENT_TYPE] = type;
        
    }
    else {
        outResponse.mCode = HTTP::Code::INTERNAL_SERVER_ERROR;
    }
}
