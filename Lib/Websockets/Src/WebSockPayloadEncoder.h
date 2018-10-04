
#ifndef _WEBSOCK_PAYLOADENCODER_H_
#define _WEBSOCK_PAYLOADENCODER_H_

#include <string>
#include <vector>

#include "MessageStream.h"
#include "WebSockFrame.h"

namespace RFC6455 {

class PayloadStringEncoder : public OS::MessageStream<std::string, Frame> {

public:
    void Write (const std::string& inData) override;

};

class PayloadBinaryEncoder : public OS::MessageStream<std::vector<uint8_t>, Frame> {

public:
    void Write (const std::vector<uint8_t>& inData) override;

};

}

#endif // _WEBSOCK_PAYLOADENCODER_H_
