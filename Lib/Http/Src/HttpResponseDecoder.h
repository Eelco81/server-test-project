
#ifndef _HTTP_RESPONSEDECODER_H_
#define _HTTP_RESPONSEDECODER_H_

#include <memory>
#include "HttpResponse.h"
#include "HttpMessageDecoder.h"

namespace HTTP {

/**
 * The ResponseDecoder converts a stream of std::string to a stream of HTTP::Response.
 */
class ResponseDecoder : public MessageDecoder<Response> {

public:
    /**
     * Constructor
     */
    ResponseDecoder ();
    
    /**
     * Virtual destructor
     */
    virtual ~ResponseDecoder ();
    
    /**
     * Override the line matcher
     */
    bool MatchStartLine (const std::string& inLine) override;
    
};

} // end namespace HTTP

#endif // end _HTTP_RESPONSEDECODER_H_