
#ifndef _RF6455_FRAME_H_
#define _RF6455_FRAME_H_

#include <cstdint>
#include <vector>
#include <string>

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
     * Create a log message summarizing the packet
     */
    std::string GetStatusMessage () const;
    
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
