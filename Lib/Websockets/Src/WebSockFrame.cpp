
#include "WebSockFrame.h"

RFC6455::Frame::Frame () :
    mFin (false),
    mOpCode (INVALID),
    mIsMasked (false)
{
}

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