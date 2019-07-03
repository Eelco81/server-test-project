
#ifndef _RFC6455_FRAMEDECODER_H_
#define _RFC6455_FRAMEDECODER_H_

#include <vector>
#include <cstdint>

#include "TcpPacket.h"
#include "MessageStream.h"
#include "WebSockFrame.h"

namespace RFC6455 {

class FrameDecoder : public OS::MessageStream <TCP::Packet, Frame>{
    
public:
    /**
     * Destructor
     */
    virtual ~FrameDecoder ();
    
    /**
     * Write buffer to the decoder
     */
    void Write (const TCP::Packet& inBuffer) override;
    
protected:
    std::vector<uint8_t> mBuffer;

};


} // end namespace RFC6455

#endif // _RFC6455_FRAMEDECODER_H_
