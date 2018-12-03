
#include <map>
#include "ApiUtils.h"
#include "FileSystem.h"

#include <json.hpp>
using json = nlohmann::json;

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

void API::Utils::SetErrorMessage (HTTP::Response& ioResponse, HTTP::Code inCode, const std::string& inMessage) {
    json j;
    j["error"] = inMessage;
    ioResponse.SetBody (j.dump (), "application/json");
    ioResponse.mCode = inCode;
}

void API::Utils::SetFileMessage (HTTP::Response& ioResponse, const std::string& inPath) {
    std::string data;
    if (OS::FileSystem::Read (inPath, data)) {
        auto type (contentTypes [OS::FileSystem::GetExtension (inPath)]);
        if (type == "") {
            type = "text/plain";
        }
        ioResponse.mCode = HTTP::Code::OK;
        ioResponse.SetBody (data, type);
    }
    else {
        SetErrorMessage (ioResponse, HTTP::Code::INTERNAL_SERVER_ERROR, "Failed to read the requested resource");
    }
}