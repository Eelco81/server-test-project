
#include <regex>
#include <iostream>
#include <sstream>
#include <string>

#include "HttpRequest.h"

HTTP::Request::Request () :
    mMethod (HTTP::Method::UNKNOWN_METHOD),
    mVersion (HTTP::Version::UNKNOWN_VERSION),
    mIsValid (true)
{
}

// Mainly a convenience constructor for unit tests
HTTP::Request::Request (HTTP::Method inMethod, const std::string& inPath) :
    mMethod (inMethod),
    mVersion (HTTP::Version::V11),
    mPath (inPath),
    mIsValid (true)
{
}

std::string HTTP::Request::ToString () const {
    auto request (MethodToString (mMethod) + std::string (" ") + mPath + std::string (" ") + std::string ("HTTP/") + VersionToString (mVersion) + std::string ("\r\n"));
    for (const auto& pair : mHeaders) {
        request += pair.first + std::string (": ") + pair.second + std::string ("\r\n");
    }
    request += std::string ("\r\n");
    request += mBody;
    return request;
}

HTTP::RequestParser::RequestParser () :
    mState (kSearchStart),
    mBodySize (0u)
{
}

HTTP::RequestParser::~RequestParser () {
}

void HTTP::RequestParser::Write (const std::string& inData) {
    
    std::istringstream iss (inData);
    for (std::string line; std::getline(iss, line); ) {
        
        switch (mState) {
            case kSearchStart : {
                std::regex reInitialLine ("^([A-Z]+)\\s([A-Za-z0-9_/\\-\\.]+)\\sHTTP/([0-9\\.0-9]+)");
                std::smatch matchLine;
                if (std::regex_search (line, matchLine, reInitialLine)) {
                    mRequest.mMethod = StringToMethod (matchLine[1].str ());
                    mRequest.mPath = matchLine[2].str ();
                    mRequest.mVersion = StringToVersion (matchLine[3].str ());
                    mRequest.mHeaders.clear();
                    mRequest.mBody = "";
                    mRequest.mIsValid = true;
                    mBodySize = 0u;
                    mState = kProcessHeaders;
                }
                break;
            }
            case kProcessHeaders : {
                if (line == "\r" || line == "") {
                    const auto contentLength (mRequest.mHeaders.find (Header::CONTENT_LENGTH));
                    if (contentLength == mRequest.mHeaders.end ()) {
                        HandleRequest (mRequest);
                        mState = kSearchStart;
                    }
                    else {
                        try {
                           mBodySize = std::stoi (contentLength->second);
                        }
                        catch (...) {
                            mBodySize = 0;
                            mRequest.mIsValid = false;
                        }
                        if (mBodySize == 0) {
                            HandleRequest (mRequest);
                            mState = kSearchStart;
                        }
                        else {
                            mState = kProcessBody;
                        }
                    }
                }
                else {
                    std::regex reHeader ("^(.+)\\s*:\\s*(.+)\r$");
                    std::smatch matchLine;
                    if (std::regex_search (line, matchLine, reHeader)) {
                        mRequest.mHeaders[matchLine[1].str ()] = matchLine[2].str ();
                    }
                }
                break;
            }
            case kProcessBody : {
                if (mRequest.mBody.size () < mBodySize) {
                    mRequest.mBody += line;
                }
                if (mRequest.mBody.size () >= mBodySize) {
                    HandleRequest (mRequest);
                    mState = kSearchStart;
                }
                break;
            }
        }
    }
}
