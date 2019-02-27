
#ifndef _OS_STRING_CONVERTER_H_
#define _OS_STRING_CONVERTER_H_

#include "MessageStream.h"

namespace OS {

/**
 * The string to packet converter transforms a std::string 
 * to a binary packet of std::vector<uint8_t>.
 */
class StringToPacketConverter : public MessageStream<std::string, std::vector<uint8_t>> {

public:

    /**
     * Write to the converter.
     */
    void Write (const std::string& inMessage) override {
        std::vector<uint8_t> packet;
        packet.assign (reinterpret_cast<const uint8_t*>(inMessage.data ()), reinterpret_cast<const uint8_t*>(inMessage.data ()) + inMessage.size ());
        Emit (packet);
    }

};

/**
 * The string to packet converter transforms a a binary 
 * packet of std::vector<uint8_t> to std::string.
 */
class PacketToStringConverter : public MessageStream<std::vector<uint8_t>, std::string> {

public:

    /**
     * Write to the converter.
     */
    void Write (const std::vector<uint8_t>& inMessage) override {
        Emit (std::string (reinterpret_cast<const char*>(inMessage.data ()), inMessage.size ()));
   }

};

} // end namespace OS

#endif // _OS_STRING_CONVERTER_H_
