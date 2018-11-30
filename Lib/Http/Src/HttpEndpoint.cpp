
#include "HttpEndpoint.h"

bool HTTP::AbstractEndPoint::Route (const Request& inRequest, Response& outResponse) {
    outResponse.mCode = Code::NOT_FOUND;
    switch (inRequest.mMethod) {
        case Method::GET :
            Get (inRequest, outResponse);
            break;
        case Method::PUT :
            Put (inRequest, outResponse);
            break;
        case Method::POST :
            Post (inRequest, outResponse);
            break;
        case Method::DELETE :
            Delete (inRequest, outResponse);
            break;
        default:
            break;
    }
    return true;
}

HTTP::EndPoint::EndPoint (const std::string& inPath) :
    mPath (inPath)
{
}

bool HTTP::EndPoint::Route (const Request& inRequest, Response& outResponse) {
    
    if (mPath != inRequest.mPath) {
        return false;
    }

    return AbstractEndPoint::Route (inRequest, outResponse);
}

HTTP::RegexEndPoint::RegexEndPoint (const std::regex& inPath) :
    mRegex (inPath)
{
}

bool HTTP::RegexEndPoint::Route (const Request& inRequest, Response& outResponse) {
    
    std::smatch match;
    if (!std::regex_match (inRequest.mPath, match, mRegex)) {
        return false; 
    }
    
    mParameterList.resize (match.size ());
    std::transform (match.begin (), match.end (), mParameterList.begin (), [](const auto& m) { return m.str(); });
    
    return AbstractEndPoint::Route (inRequest, outResponse);
}