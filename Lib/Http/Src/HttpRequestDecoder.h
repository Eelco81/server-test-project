
#ifndef _HTTP_REQUESTDECODER_H_
#define _HTTP_REQUESTDECODER_H_

#include "HttpRequest.h"

namespace HTTP {
    
class RequestDecoder {
    
public:
    RequestDecoder ();
    virtual ~RequestDecoder ();
    void Write (const std::string& inData);

protected:
    virtual void HandleRequest (const Request& inRequest) = 0;
    
private:
    enum State : uint8_t {
        kSearchStart = 0x00,
        kProcessHeaders = 0x01,
        kProcessBody = 0x02
    };
    
    State mState;
    Request mRequest;
    std::size_t mBodySize;
};

} // end namespace HTTP

#endif // end _HTTP_REQUESTDECODER_H_
