
#include <regex>
#include <iostream>
#include "HttpRequest.h"

HTTP::Request::Request () :
    mMethod (HTTP::Method::UNKNOWN_METHOD),
    mVersion (HTTP::Version::UNKNOWN_VERSION)
{
}

const HTTP::Method& HTTP::Request::GetMethod () const { 
    return mMethod;
}

const HTTP::Version& HTTP::Request::GetVersion () const { 
    return mVersion;
}

const HTTP::Request::HeaderMap& HTTP::Request::GetHeaders () const { 
    return mHeaders;
}

bool HTTP::Request::Parse (const std::string& inData) {
    
    // Find method
    std::regex reMethod ("^([A-Z]+)");
    std::smatch matchMethod;
    if (!std::regex_search (inData, matchMethod, reMethod)) {
        return false;
    }
    // std::cout << matchMethod.str() << std::endl;
    mMethod = StringToMethod (matchMethod.str ());
    
    // Find version
    std::regex reVersion ("HTTP/[0-9]\\.[0-9]");
    std::smatch matchVersion;
    if (!std::regex_search (inData, matchVersion, reVersion)) {
        return false;
    }
    std::string version (matchVersion.str());
    version.erase (version.begin(), version.begin() + 5);
    // std::cout << version << std::endl;
    mVersion = StringToVersion (version);
    
    //std::regex re ("^([A-Z]+)\\s+([A-Za-z0-9_/\\-\\.]+\\s+HTTP/[0-9\\.]+)");
    
    return true;
}