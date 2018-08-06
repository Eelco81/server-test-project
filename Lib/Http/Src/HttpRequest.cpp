
#include <regex>
#include <iostream>
#include <sstream>
#include "HttpRequest.h"

HTTP::Request::Request () :
    mMethod (HTTP::Method::UNKNOWN_METHOD),
    mVersion (HTTP::Version::UNKNOWN_VERSION)
{
}

void HTTP::RequestParser::Write (const std::string& inData) {
    
    std::istringstream iss (inData);
    for (std::string line; std::getline(iss, line); ) {
        
        std::regex reInitialLine ("^([A-Z]+)\\s([A-Za-z0-9_/\\-\\.]+)\\sHTTP/([0-9\\.0-9]+)");
        std::smatch matchLine;
        if (std::regex_search (line, matchLine, reInitialLine)) {
            Request request;
            request.mMethod = StringToMethod (matchLine[1].str ());
            request.mPath = matchLine[2].str ();
            request.mVersion = StringToVersion (matchLine[3].str ());
            HandleRequest (request);
        }
    }
    
}
