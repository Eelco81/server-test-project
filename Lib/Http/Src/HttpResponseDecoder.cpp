
#include <regex>
#include <iostream>
#include <sstream>
#include <string>

#include "HttpRequest.h"
#include "HttpResponseDecoder.h"

HTTP::ResponseDecoder::ResponseDecoder () = default;
HTTP::ResponseDecoder::~ResponseDecoder () = default;

bool HTTP::ResponseDecoder::MatchStartLine (const std::string& inLine) {
    
    std::regex reInitialLine ("^HTTP/([0-9\\.0-9]+)\\s([0-9]+)");
    std::smatch matchLine;
    if (std::regex_search (inLine, matchLine, reInitialLine)) {
        
        mMessage = std::make_unique<Response> ();
        mMessage->mVersion = StringToVersion (matchLine[1].str ());
        mMessage->mCode = StringToCode (matchLine[2].str ());
        
        return true;
    }
    
    return false;
}
