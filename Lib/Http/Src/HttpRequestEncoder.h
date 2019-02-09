
#ifndef _HTTP_REQUESTENCODER_H_
#define _HTTP_REQUESTENCODER_H_

#include "HttpRequest.h"
#include "MessageStream.h"

namespace HTTP {

/**
 * The RequestEncoder converts a stream of HTTP::Request to a stream of std::string.
 */
class RequestEncoder : public OS::MessageStream<Request, std::string> {

public:
    /**
     * Write a Request to the Encoder, when a corresponding string is assembled,
     * the MessageStream::Done method is called.
     */ 
    void Write (const Request& inData);

};

} // end namespace HTTP

#endif // _HTTP_REQUESTENCODER_H_
