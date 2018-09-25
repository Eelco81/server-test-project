
#include "WebSockFrame.h"

RFC6455::Frame::Frame () :
    mFin (false),
    mOpCode (0x00),
    mIsMasked (false)
{
}

void RFC6455::Frame::SetFin (bool inFin) {
    mFin = inFin;
}

bool RFC6455::Frame::GetFin () const {
    return mFin;
}

void RFC6455::Frame::SetOpCode (uint8_t inOpCode) {
    mOpCode = inOpCode;
}

uint8_t RFC6455::Frame::GetOpCode () const {
    return mOpCode;
}

void RFC6455::Frame::SetIsMasked (bool inIsMasked) {
    mIsMasked = inIsMasked;
}

bool RFC6455::Frame::GetIsMasked () const {
    return mIsMasked;
}

void RFC6455::Frame::SetPayload (uint8_t* inData, std::size_t inSize) {
    mPayload.assign (inData, inData + inSize);
}

const std::vector<uint8_t>& RFC6455::Frame::GetPayload () const {
    return mPayload;
}

void RFC6455::Frame::SetMask (uint8_t* inData, std::size_t inSize) {
    mMask.assign (inData, inData + inSize);
}

const std::vector<uint8_t>& RFC6455::Frame::GetMask () const {
    return mMask;
}
