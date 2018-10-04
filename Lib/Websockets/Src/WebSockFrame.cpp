
#include "WebSockFrame.h"

RFC6455::Frame::Frame () :
    mFin (false),
    mOpCode (INVALID),
    mIsMasked (false)
{
}

RFC6455::Frame::Frame (const Frame& inFrame) :
    mFin (inFrame.mFin),
    mOpCode (inFrame.mOpCode),
    mIsMasked (inFrame.mIsMasked)
{
    mMask.assign (inFrame.mMask.begin (), inFrame.mMask.end ());
    mPayload.assign (inFrame.mPayload.begin (), inFrame.mPayload.end ());
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