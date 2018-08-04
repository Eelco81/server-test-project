

#include "Mutex.h"

void OS::Mutex::Lock () {
    mMutex.lock ();
}

void OS::Mutex::UnLock () {
    mMutex.unlock ();
}