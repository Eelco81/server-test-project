
#ifndef _RFC6455_PAYLOADDECODER_H_
#define _RFC6455_PAYLOADDECODER_H_

#include <cstdint>
#include <vector>
#include <string>
#include "MessageStream.h"
#include "WebSockFrame.h"

namespace RFC6455 {

class PayloadStringDecoder : public OS::MessageStream <Frame, std::string> {

public:
    /**
     * Default constructor
     */
    PayloadStringDecoder () = default;
    
    /**
     * Default (virtual) destructor
     */
    virtual ~PayloadStringDecoder () = default;
    
    /**
     * Write frame to the stream
     */
    void Write (const Frame& inFrame) override;

private:
    std::vector<uint8_t> mBuffer;
    bool mIsDecoding = false;
};

class PayloadBinaryDecoder : public OS::MessageStream <Frame, std::vector<uint8_t>> {

public:
    /**
     * Default constructor
     */
    PayloadBinaryDecoder () = default;
    
    /**
     * Default (virtual) destructor
     */
    virtual ~PayloadBinaryDecoder () = default;
    
    /**
     * Write frame to the stream
     */
    void Write (const Frame& inFrame) override;

private:
    std::vector<uint8_t> mBuffer;
    bool mIsDecoding = false;
};

} // end namespace RFC6455

#endif //_RFC6455_PAYLOADDECODER_H_
