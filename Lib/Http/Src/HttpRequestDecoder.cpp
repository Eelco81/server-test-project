
#include <regex>
#include <iostream>
#include <sstream>
#include <string>

#include "HttpRequest.h"
#include "HttpRequestDecoder.h"

HTTP::RequestDecoder::RequestDecoder () = default;
HTTP::RequestDecoder::~RequestDecoder () = default;

bool HTTP::RequestDecoder::MatchStartLine (const std::string& inLine) {
    
    std::regex reInitialLine ("^([A-Z]+)\\s([A-Za-z0-9_/\\-\\.]+)\\sHTTP/([0-9\\.0-9]+)");
    std::smatch matchLine;
    if (std::regex_search (inLine, matchLine, reInitialLine)) {
        
        mMessage = std::make_unique<Request> ();
        mMessage->mMethod = StringToMethod (matchLine[1].str ());
        mMessage->mPath = matchLine[2].str ();
        mMessage->mVersion = StringToVersion (matchLine[3].str ());
        
        return true;
    }
    
    return false;
}
