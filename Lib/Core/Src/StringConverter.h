
#ifndef _OS_STRING_CONVERTER_H_
#define _OS_STRING_CONVERTER_H_

#include "MessageStream.h"

namespace OS {


class StringToPacketConverter : public MessageStream<std::string, std::vector<uint8_t>> {

public:
    void Write (const std::string& inMessage) override {
        std::vector<uint8_t> packet;
        packet.assign (reinterpret_cast<const uint8_t*>(inMessage.data ()), reinterpret_cast<const uint8_t*>(inMessage.data ()) + inMessage.size ());
        Done (packet);
    }

};

class PacketToStringConverter : public MessageStream<std::vector<uint8_t>, std::string> {

public:
    void Write (const std::vector<uint8_t>& inMessage) override {
        Done (std::string (reinterpret_cast<const char*>(inMessage.data ()), inMessage.size ()));
   }

};

} // end namespace OS

#endif // _OS_STRING_CONVERTER_H_
