
#ifndef _HTTP_REQUESTDECODER_H_
#define _HTTP_REQUESTDECODER_H_

#include <memory>
#include "HttpRequest.h"
#include "MessageStream.h"

namespace HTTP {

/**
 * The RequestDecoder converts a stream of std::string to a stream of HTTP::Request.
 */
class RequestDecoder : public OS::MessageStream<std::string, Request> {

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
     * Write a string to the Decoder, when a corresponding Request is assembled,
     * the MessageStream::Done method is called.
     */ 
    void Write (const std::string& inData);

private:
    /**
     * The request being assembled
     */
    std::unique_ptr<Request> mRequest;
    
    /**
     * State of the decoder
     */
    enum State : uint8_t {
        kSearchStart = 0x00,
        kProcessHeaders = 0x01,
        kProcessBody = 0x02
    };
    State mState;
    
    /**
     * The body size of the currently assembled request.
     */
    std::size_t mBodySize;
    std::string mBody;
};

} // end namespace HTTP

#endif // end _HTTP_REQUESTDECODER_H_
