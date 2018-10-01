
#include <regex>
#include <iostream>
#include <sstream>
#include <string>

#include "HttpRequest.h"
#include "HttpRequestDecoder.h"

HTTP::RequestDecoder::RequestDecoder () :
    mState (kSearchStart),
    mBodySize (0u)
{
}

HTTP::RequestDecoder::~RequestDecoder () = default;

void HTTP::RequestDecoder::Write (const std::string& inData) {
    
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
                        Done (mRequest);
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
                            Done (mRequest);
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
                    Done (mRequest);
                    mState = kSearchStart;
                }
                break;
            }
        }
    }
}
