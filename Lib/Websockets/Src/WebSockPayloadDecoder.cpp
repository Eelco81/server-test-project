
#include <sstream>
#include <iostream>
#include "WebSockPayloadDecoder.h"

namespace {

// Note: the inverse of an xor is again an xor. So encoding and decoding is the same operation.
void Encode (const std::vector<uint8_t>& inMask, std::vector<uint8_t>& ioPayload) {
    for (std::size_t i (0u); i < ioPayload.size (); i++) {
        ioPayload[i] = ioPayload[i] ^ inMask[i % inMask.size ()];
    }
}

} // end anonymous namespace

void RFC6455::PayloadStringDecoder::Write (const Frame& inFrame) {

    if (inFrame.mOpCode == Frame::OpCode::TEXT) {
        
        mBuffer.assign (inFrame.mPayload.begin (), inFrame.mPayload.end ());
        
        if (inFrame.mIsMasked) {
            Encode (inFrame.mMask, mBuffer);
        }
        
        if (inFrame.mFin) {
            Done (std::string (reinterpret_cast<char*> (mBuffer.data()), mBuffer.size ()));
            mIsDecoding = false;
        }
        else {
            mIsDecoding = true;
        }
        
    }
    else if (inFrame.mOpCode == Frame::OpCode::CONT && mIsDecoding) {
        
        std::vector<uint8_t> payload (inFrame.mPayload);
        
        if (inFrame.mIsMasked) {
            Encode (inFrame.mMask, payload);
        }
        
        std::copy (payload.begin (), payload.end (), std::back_inserter (mBuffer));
        
        if (inFrame.mFin) {
            Done (std::string (reinterpret_cast<char*> (mBuffer.data())));
            mIsDecoding = false;
        }
    }
    else {
        mIsDecoding = false;
        // Ignore frame.
    }

}

void RFC6455::PayloadBinaryDecoder::Write (const Frame& inFrame) {

    if (inFrame.mOpCode == Frame::OpCode::BIN) {
        
        mBuffer.assign (inFrame.mPayload.begin (), inFrame.mPayload.end ());
        
        if (inFrame.mIsMasked) {
            Encode (inFrame.mMask, mBuffer);
        }
        
        if (inFrame.mFin) {
            Done (mBuffer);
            mIsDecoding = false;
        }
        else {
            mIsDecoding = true;
        }
        
    }
    else if (inFrame.mOpCode == Frame::OpCode::CONT && mIsDecoding) {
        
        std::vector<uint8_t> payload (inFrame.mPayload);
        
        if (inFrame.mIsMasked) {
            Encode (inFrame.mMask, payload);
        }
        
        std::copy (payload.begin (), payload.end (), std::back_inserter (mBuffer));
        
        if (inFrame.mFin) {
            Done (mBuffer);
            mIsDecoding = false;
        }
    }
    else {
        mIsDecoding = false;
        // Ignore frame.
    }

}

