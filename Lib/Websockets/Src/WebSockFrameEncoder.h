
#ifndef _RFC6455_FRAMEENCODER_H_
#define _RFC6455_FRAMEENCODER_H_

#include <vector>
#include <cstdint>

#include "TcpPacket.h"
#include "MessageStream.h"
#include "WebSockFrame.h"

namespace RFC6455 {

/**
 * The FrameEncoder streams Websocket Frames to TCP::Packets.
 */
class FrameEncoder : public OS::MessageStream <Frame, TCP::Packet> {

public:
    /**
     * Write Frame to the encoder
     */
    void Write (const Frame& inBuffer) override;

};

} // end namespace RFC6455

#endif // _RFC6455_FRAMEENCODER_H_
