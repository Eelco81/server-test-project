

#include "Mutex.h"

void OS::Mutex::Lock () {
    mMutex.lock ();
}

void OS::Mutex::UnLock () {
    mMutex.unlock ();
}

OS::SingleLock::SingleLock (OS::Mutex& inMutex) :
    mMutex (inMutex) 
{
    mMutex.Lock ();
}

OS::SingleLock::~SingleLock () {
    mMutex.UnLock ();
}