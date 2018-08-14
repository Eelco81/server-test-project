
#ifndef _HTTP_REQUEST_H
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
    HeaderMap mHeaders;
    std::string mPath;
    std::string mBody;
    bool mIsValid;
};

class RequestParser {
    
public:
    void Write (const std::string& inData);

protected:
    virtual void HandleRequest (const Request& inRequest) = 0;
    
};

} // end namespace HTTP

#endif // _HTTP_REQUEST_H_
