
#ifndef _RFC6455_FRAMEENCODER_H_
#define _RFC6455_FRAMEENCODER_H_

#include <vector>
#include <cstdint>

#include "MessageStream.h"
#include "WebSockFrame.h"

namespace RFC6455 {
    
class FrameEncoder : public OS::MessageStream <Frame, std::vector<uint8_t>>{

public:
    /**
     * Write Frame to the encoder
     */
    void Write (const Frame& inBuffer) override;

};

} // end namespace RFC6455

#endif // _RFC6455_FRAMEENCODER_H_
