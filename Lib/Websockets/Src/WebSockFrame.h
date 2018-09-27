
#ifndef _RF6455_FRAME_H_
#define _RF6455_FRAME_H_

#include <cstdint>
#include <vector>

namespace RFC6455 {

struct Frame {
    
    /**
     * OpCode defined in RFC6455
     */
    enum OpCode : uint8_t {
        CONT = 0x0,
        TEXT = 0x1,
        BIN = 0x2,
        CLOSE = 0x8,
        PING = 0x9,
        PONG = 0xA,
        INVALID = 0xFF
    };
    
    /**
     * Constructor
     */
    Frame ();
    
    /**
     * Decode the payload
     */
    void Decode ();
    
    /**
     * Encode the payload
     */
    void Encode ();
    
    /**
     * Create a buffer out of the data
     */
    std::vector<uint8_t> ToBuffer () const;
    
    /**
     * Data members
     */
    bool mFin;
    uint8_t mOpCode;
    bool mIsMasked;
    std::vector<uint8_t> mMask;
    std::vector<uint8_t> mPayload;
    
};

} // end namespace RFC6455

#endif // _RF6455_FRAME_H_
