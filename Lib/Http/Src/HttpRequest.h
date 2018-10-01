
#ifndef _HTTP_REQUEST_H_
#define _HTTP_REQUEST_H_

#include <cstdint>
#include <string>
#include <map>

#include "HttpCodes.h"

namespace HTTP {

struct Request {
    
    using HeaderMap = std::map<std::string,std::string>;
    
    Request ();
    Request (Method inMethod, const std::string& inPath);
    
    std::string ToString () const;
    
    Method mMethod;
    Version mVersion;
    HeaderMap mHeaders; //todo: header keys are not case sensitive
    std::string mPath;
    std::string mBody;
    bool mIsValid;
};

} // end namespace HTTP

#endif // _HTTP_REQUEST_H_
