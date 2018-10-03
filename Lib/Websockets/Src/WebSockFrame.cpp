
#include "WebSockFrame.h"

RFC6455::Frame::Frame () :
    mFin (false),
    mOpCode (INVALID),
    mIsMasked (false)
{
}

// The inverse of an XOR is itself
/*
void RFC6455::Frame::Decode () {
    
    Encode ();
}

void RFC6455::Frame::Encode () {
    
    if (mIsMasked && mMask.size () == 4u) {
        for (std::size_t i (0u); i < mPayload.size (); i++) {
            mPayload[i] ^= mMask[i % 4u];
        }
    }
}
*/

std::string RFC6455::Frame::GetStatusMessage () const {
    return std::string ("RFC6455 FIN[") + 
        std::to_string (mFin) + 
        std::string ("] OpCode [") + 
        std::to_string (mOpCode) + 
        std::string ("] Masked[") +
        std::to_string (mIsMasked) + 
        std::string ("] Size[") +
        std::to_string (mPayload.size ()) + 
        std::string ("]");
}