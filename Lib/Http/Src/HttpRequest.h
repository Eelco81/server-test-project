
#ifndef _HTTP_REQUEST_H_
#define _HTTP_REQUEST_H_

#include <map>
#include <string>
#include <vector>
#include <cstdint>
#include "HttpCodes.h"
#include "HttpMessage.h"

namespace HTTP {

/**
 * The HTTP::Request struct contains a complete request message of the HTTP protocol.
 */
struct Request : public Message {
    
    /**
     * Constructors
     */
    Request ();
    Request (Method inMethod, const std::string& inPath);
    Request (Method inMethod, const std::string& inPath, Version inVersion);
    
    /**
     * HTTP Method
     */
    Method mMethod;
    
    /**
     * HTTP Version
     */
    Version mVersion;
    
    /**
     * HTTP Path
     */
    std::string mPath;
    
    /**
     * Internal boolean used by the request decoder.
     */
    bool mIsValid;
    
};

} // end namespace HTTP

#endif // _HTTP_REQUEST_H_
