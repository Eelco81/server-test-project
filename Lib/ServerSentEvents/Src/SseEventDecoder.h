
#ifndef _SSE_EVENTDECODER_H_
#define _SSE_EVENTDECODER_H_

#include <vector>
#include <cstdint>

#include "MessageStream.h"

namespace SSE {
/**
 * The SSE::EventDecoder extracts and pipes the payload of Server-Sent-Events.
 */
class EventDecoder : public OS::MessageStream <std::string, std::string>{
    
public:
    /**
     * Destructor
     */
    virtual ~EventDecoder ();
    
    /**
     * Write buffer to the decoder
     */
    void Write (const std::string& inBuffer) override;
    
protected:
    std::string mBuffer;
};


} // end namespace SSE

#endif // _SSE_EVENTDECODER_H_
