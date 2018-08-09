
#ifndef _HTTP_RESPONSE_H_
#define _HTTP_RESPONSE_H_

#include <map>
#include <string>
#include "HttpCodes.h"

namespace HTTP {
    
struct Response {
    
    using HeaderMap = std::map<std::string,std::string>;
    
    Response ();
    Response (Code inCode, Version inVersion);
    
    std::string ToString () const;
    
    Code mCode;
    Version mVersion;
    HeaderMap mHeaders;
    std::string mBody;
};

}

#endif // _HTTP_RESPONSE_H_
