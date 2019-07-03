
#include "WebSockPayloadEncoder.h"

void RFC6455::PayloadStringEncoder::Write (const std::string& inData) {
    
    Frame frame;
    frame.mFin = true;
    frame.mOpCode = Frame::OpCode::TEXT;
    frame.mIsMasked = false;
    frame.mPayload.assign (reinterpret_cast<const uint8_t*> (inData.data ()), reinterpret_cast<const uint8_t*> (inData.data ()) + inData.size ());
    
    sNext.Emit (frame);
}

void RFC6455::PayloadBinaryEncoder::Write (const std::vector<uint8_t>& inData) {
    
    Frame frame;
    frame.mFin = true;
    frame.mOpCode = Frame::OpCode::BIN;
    frame.mIsMasked = false;
    frame.mPayload.assign (inData.begin (), inData.end ());
    
    sNext.Emit (frame);
}
