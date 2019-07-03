
#ifndef _TCP_PACKET_ENCODER_
#define _TCP_PACKET_ENCODER_

#include "TcpPacket.h"
#include "MessageStream.h"

namespace TCP { 


/**
    * Convert a TCP::RawPacket a TCP::Packet
    */
class RawPacket2PacketEncoder : public OS::MessageStream<RawPacket, Packet> {
public:
    void Write (const RawPacket& inData) override {
        Packet packet;
        packet.assign (std::get<0> (inData), std::get<0> (inData) + std::get<1> (inData));
        sNext.Emit (packet);
    }
};


/**
 * Convert a string to a TCP::RawPacket
 */
class String2PacketEncoder : public OS::MessageStream<std::string, RawPacket> {
public:
    void Write (const std::string& inData) override {
        sNext.Emit (RawPacket (reinterpret_cast<const uint8_t*> (inData.c_str()), inData.size()));
    }
};

/**
 * Convert a TCP::RawPacket to std::string
 */
class Packet2StringEncoder : public OS::MessageStream<RawPacket, std::string> {
public:
    void Write (const RawPacket& inData) override {
        sNext.Emit (std::string (reinterpret_cast<const char*> (std::get<0> (inData)), std::get<1> (inData)));
    }
};

} // end namespace TCP

#endif // _TCP_PACKET_ENCODER_
