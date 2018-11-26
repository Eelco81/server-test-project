
#include "HttpEndpoint.h"
#include <iostream>

HTTP::EndPoint::EndPoint (const std::string& inPath, HTTP::Method inMethod) :
    mPath (inPath),
    mMethod (inMethod)
{
}

HTTP::EndPoint::EndPoint (const std::regex& inPath, HTTP::Method inMethod) :
    mRegex (inPath),
    mMethod (inMethod)
{
}

HTTP::EndPoint::~EndPoint () = default;

bool HTTP::EndPoint::Route (const Request& inRequest, Response& outResponse) {
    
    if (inRequest.mMethod != mMethod) {
        return false;
    }
    
    mParameterList.clear ();
    
    if (!mPath.empty() && mPath == inRequest.mPath) {
        
        outResponse.mCode = Code::INTERNAL_SERVER_ERROR;
        Execute (inRequest, outResponse);
        return true;
    }
    
    std::smatch match;
    if (mPath.empty() && std::regex_match (inRequest.mPath, match, mRegex)) {
        
        mParameterList.resize (match.size ());
        std::transform (match.begin (), match.end (), mParameterList.begin (), [](const auto& m) { return m.str(); });
        
        outResponse.mCode = Code::INTERNAL_SERVER_ERROR;
        Execute (inRequest, outResponse);
        return true;
    }
    
    return false;
}
