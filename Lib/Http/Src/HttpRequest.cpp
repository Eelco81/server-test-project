
#include <regex>
#include <iostream>
#include <sstream>
#include <string>

#include "HttpRequest.h"

HTTP::Request::Request () :
    mMethod (HTTP::Method::UNKNOWN_METHOD),
    mVersion (HTTP::Version::UNKNOWN_VERSION)
{
}

void HTTP::RequestParser::Write (const std::string& inData) {
    
    enum State : uint8_t {
        kSearchStart = 0x00,
        kProcessHeaders = 0x01,
        kProcessBody = 0x02
    };
    
    State state (kSearchStart);
    Request request;
    std::size_t bodySize (0u);
    
    std::istringstream iss (inData);
    for (std::string line; std::getline(iss, line); ) {
        
        switch (state) {
            case kSearchStart : {
                std::regex reInitialLine ("^([A-Z]+)\\s([A-Za-z0-9_/\\-\\.]+)\\sHTTP/([0-9\\.0-9]+)$");
                std::smatch matchLine;
                if (std::regex_search (line, matchLine, reInitialLine)) {
                    request.mMethod = StringToMethod (matchLine[1].str ());
                    request.mPath = matchLine[2].str ();
                    request.mVersion = StringToVersion (matchLine[3].str ());
                    request.mHeaders.clear();
                    request.mBody = "";
                    state = kProcessHeaders;
                    bodySize = 0u;
                }
                break;
            }
            case kProcessHeaders : {
                if (line == "") {
                    const auto contentLength (request.mHeaders.find ("Content-Length"));
                    if (contentLength == request.mHeaders.end ()) {
                        HandleRequest (request);
                        state = kSearchStart;
                    }
                    else {
                        bodySize = std::stoi (contentLength->second);
                        state = kProcessBody;
                    }
                }
                else {
                    std::regex reHeader ("^(.+)\\s*:\\s*(.+)$");
                    std::smatch matchLine;
                    if (std::regex_search (line, matchLine, reHeader)) {
                        request.mHeaders[matchLine[1].str ()] = matchLine[2].str ();
                    }
                }
                break;
            }
            case kProcessBody : {
                if (request.mBody.size () < bodySize) {
                    request.mBody += line;
                }
                if (request.mBody.size () >= bodySize) {
                    HandleRequest (request);
                    state = kSearchStart;
                }
                break;
            }
            
        }
    }
}
