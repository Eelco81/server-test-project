
#ifndef _HTTP_RESPONSE_H_
#define _HTTP_RESPONSE_H_

#include <map>
#include <string>
#include <vector>
#include <cstdint>
#include "HttpCodes.h"

namespace HTTP {
    
struct Response {
    
    using HeaderMap = std::map<std::string,std::string>;
    
    Response ();
    Response (Code inCode, Version inVersion);
    
    void SetBody (const std::string& inBody, const std::string& inContentType = "text/plain");
    void SetBody (const std::vector<uint8_t>& inBuffer, const std::string& inContentType = "application/octet-stream");
    const std::string& GetBody() const;
    
    std::string ToString () const;
    
    Code mCode;
    Version mVersion;
    HeaderMap mHeaders;  //todo: header keys are not case sensitive

protected:
    std::string mBody;
};

}

#endif // _HTTP_RESPONSE_H_
