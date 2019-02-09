
#ifndef _HTTP_RESPONSEENCODER_H_
#define _HTTP_RESPONSEENCODER_H_

#include "HttpResponse.h"
#include "MessageStream.h"

namespace HTTP {

/**
 * The RequestEncoder converts a stream of HTTP::Response to a stream of std::string.
 */
class ResponseEncoder : public OS::MessageStream<Response, std::string> {

public:
    /**
     * Write a Response to the Encoder, when a corresponding string is assembled,
     * the MessageStream::Done method is called.
     */ 
    void Write (const Response& inData);

};

} // end namespace HTTP

#endif // _HTTP_RESPONSEENCODER_H_
