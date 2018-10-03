

#include "WebSockFrame.h"
#include "WebSockFrameEncoder.h"

void RFC6455::FrameEncoder::Write (const RFC6455::Frame& inFrame) {
    
    std::vector<uint8_t> buffer (2u, 0u);
    
    buffer[0] = 0b10000000 * inFrame.mFin;
    buffer[0] += inFrame.mOpCode;
    buffer[1] = 0b10000000 * inFrame.mIsMasked;
    
    const auto payloadSize (inFrame.mPayload.size ());
    
    if (payloadSize < 0x7E) {
        buffer[1] += payloadSize;
    }
    else if (payloadSize <= 0xFFFF) {
        buffer[1] += 0x7E;
        buffer.push_back (payloadSize >> 8);
        buffer.push_back (payloadSize - ((payloadSize >> 8) << 8));
    }
    else {
        buffer[1] += 0x7F;
        for (int i = 0; i < 8; i++) {
            buffer.push_back (uint8_t((payloadSize >> 8*(7 - i)) & 0xFF));
        }
    }
    
    if (inFrame.mIsMasked && inFrame.mMask.size () == 4u) {
        std::copy (inFrame.mMask.begin (), inFrame.mMask.end (), std::back_inserter (buffer));
    }
    
    std::copy (inFrame.mPayload.begin (), inFrame.mPayload.end (), std::back_inserter (buffer));
    
    Done (buffer);
}
