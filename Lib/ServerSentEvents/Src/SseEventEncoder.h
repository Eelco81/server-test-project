
#ifndef _SSE_EVENTENCODER_H_
#define _SSE_EVENTENCODER_H_

#include <string>
#include <cstdint>

#include "MessageStream.h"

namespace SSE {

/**
 * The SSE::EventEncoder pipes string message payloads to Server-Sent-Events encoded
 * event formats.
 */
class EventEncoder : public OS::MessageStream <std::string, std::string> {

public:
    /**
     * Write string Event to the encoder
     */
    void Write (const std::string& inPayload) override;

    /**
     * Set last id
     */
    void SetLastId (uint64_t inLastId);
    
private: 
    /**
     * Keep track of last ID.
     */
    uint64_t mLastId = 0u;
    
};

} // end namespace SSE

#endif // _SSE_EVENTENCODER_H_
