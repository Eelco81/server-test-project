
#include "Signals.h"

OS::Connection::Connection () :
    mPtr (std::shared_ptr<AbstractSignal> (nullptr)),
    mIndex (0u)
{
}

OS::Connection::Connection (const OS::Connection& inConnection) :
    mPtr (inConnection.mPtr),
    mIndex (inConnection.mIndex)
{
}

OS::Connection::Connection (const std::shared_ptr<AbstractSignal>& inPtr, uint64_t inIndex) :
    mPtr (inPtr),
    mIndex (inIndex)
{
}

void OS::Connection::Close () {
    auto lockedPtr = mPtr.lock();
    if (lockedPtr) { 
        lockedPtr->Disconnect (mIndex);
    }
}
