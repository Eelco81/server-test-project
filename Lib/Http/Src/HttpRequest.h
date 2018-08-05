
#ifndef _HTTP_REQUEST_H
#define _HTTP_REQUEST_H_

#include <string>
#include <map>

#include "HttpCodes.h"

namespace HTTP {

class Request {
    
public:

    using HeaderMap = std::map<std::string,std::string>;
    
    Request ();
    
    bool Parse (const std::string& inData);
    
    const Method& GetMethod () const;
    const Version& GetVersion () const;
    const HeaderMap& GetHeaders () const;
    
private:
    Method mMethod;
    Version mVersion;
    HeaderMap mHeaders;
};

} // end namespace HTTP

#endif // _HTTP_REQUEST_H_
