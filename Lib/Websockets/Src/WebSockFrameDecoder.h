
#ifndef _RFC6455_FRAMEDECODER_H_
#define _RFC6455_FRAMEDECODER_H_

#include <vector>
#include <cstdint>

#include "MessageStream.h"
#include "WebSockFrame.h"

namespace RFC6455 {

class FrameDecoder : public OS::MessageStream <std::vector<uint8_t>, Frame>{
    
public:
    /**
     * Destructor
     */
    virtual ~FrameDecoder ();
    
    /**
     * Write buffer to the decoder
     */
    void Write (const std::vector<uint8_t>& inBuffer) override;
    
protected:
    std::vector<uint8_t> mBuffer;

};


} // end namespace RFC6455

#endif // _RFC6455_FRAMEDECODER_H_
