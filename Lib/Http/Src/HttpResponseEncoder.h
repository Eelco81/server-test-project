
#ifndef _HTTP_RESPONSEENCODER_H_
#define _HTTP_RESPONSEENCODER_H_

#include "HttpResponse.h"
#include "MessageStream.h"

namespace HTTP {
    
class ResponseEncoder : public OS::MessageStream<Response, std::string> {
    
public:
    void Write (const Response& inData);

};

} // end namespace HTTP




#endif // _HTTP_RESPONSEENCODER_H_