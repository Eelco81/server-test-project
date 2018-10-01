
#ifndef _OS_STRING_CONVERTER_H_
#define _OS_STRING_CONVERTER_H_

#include "MessageStream.h"

namespace OS {

/*
class StringToBufferConverter : public MessageStream<std::string, std::vector<uint8_t>> {

public:
    void Write (const std::string& inMessage) {
        Done (std::vector<uint8_t> 
    }

};

*/

class PacketToStringConverter : public MessageStream<std::vector<uint8_t>, std::string> {

public:
    void Write (const std::vector<uint8_t>& inMessage) {
        Done (std::string (reinterpret_cast<const char*>(inMessage.data ()), inMessage.size ()));
   }

};

} // end namespace OS

#endif // _OS_STRING_CONVERTER_H_
