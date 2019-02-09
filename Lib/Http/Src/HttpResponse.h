
#ifndef _HTTP_RESPONSE_H_
#define _HTTP_RESPONSE_H_

#include <map>
#include <string>
#include <vector>
#include <cstdint>
#include "HttpCodes.h"
#include "HttpMessage.h"

namespace HTTP {

/**
 * The HTTP::Response struct contains a complete response message of the HTTP protocol.
 */
struct Response : public Message {
    
    /**
     * Constructors
     */
    Response ();
    Response (Code inCode, Version inVersion);
    
    /**
     * HTTP Response Code
     */
    Code mCode;
    
    /**
     * HTTP Version
     */
    Version mVersion;
};

}

#endif // _HTTP_RESPONSE_H_
