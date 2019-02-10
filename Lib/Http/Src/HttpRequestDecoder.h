
#ifndef _HTTP_REQUESTDECODER_H_
#define _HTTP_REQUESTDECODER_H_

#include <memory>
#include "HttpRequest.h"
#include "HttpMessageDecoder.h"

namespace HTTP {

/**
 * The RequestDecoder converts a stream of std::string to a stream of HTTP::Request.
 */
class RequestDecoder : public MessageDecoder<Request> {

public:
    /**
     * Constructor
     */
    RequestDecoder ();
    
    /**
     * Virtual destructor
     */
    virtual ~RequestDecoder ();
    
    /**
     * Override the line matcher
     */
    bool MatchStartLine (const std::string& inLine) override;
    
};

} // end namespace HTTP

#endif // end _HTTP_REQUESTDECODER_H_
