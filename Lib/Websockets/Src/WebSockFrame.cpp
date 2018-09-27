
#include "WebSockFrame.h"

RFC6455::Frame::Frame () :
    mFin (false),
    mOpCode (INVALID),
    mIsMasked (false)
{
}

// The inverse of an XOR is itself
void RFC6455::Frame::Decode () {
    
    Encode ();
}

void RFC6455::Frame::Encode () {
    
    if (mIsMasked && mMask.size () == 4u) {
        for (std::size_t i (0u); i < mPayload.size (); i++) {
            mPayload[i] ^= mMask[i % 4u];
        }
    }
}

std::vector<uint8_t> RFC6455::Frame::ToBuffer () const {
    
    std::vector<uint8_t> buffer (2u, 0u);
    
    buffer[0] = 0b10000000 * mFin;
    buffer[0] += mOpCode;
    buffer[1] = 0b10000000 * mIsMasked;
    
    const auto payloadSize (mPayload.size ());
    
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
    
    if (mIsMasked && mMask.size () == 4u) {
        std::copy (mMask.begin (), mMask.end (), std::back_inserter (buffer));
    }
    
    std::copy (mPayload.begin (), mPayload.end (), std::back_inserter (buffer));
    
    return buffer;

}
