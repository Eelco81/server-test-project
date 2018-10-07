
#include "Log.h"
#include "Thread.h"

OS::Thread::Thread (const std::string& inName) :
    mName (inName),
    mStatus (kUnstarted)
{
}

OS::Thread ::~Thread () = default;

void OS::Thread::Run () {
    LOGTRACE << "[Thread](" << GetName () << ") started";
    mStatus = kRunning;
    Execute ();
    mStatus = kDone;
    LOGTRACE << "[Thread](" << GetName () << ") stopped";
}

void OS::Thread::Spawn () {
    mImplementation.reset (new std::thread ([](Thread* inThread) { inThread->Run(); }, this));
}

void OS::Thread::Join () {
    if (mImplementation && mImplementation->joinable ()) {
        LOGTRACE << "Joining [Thread](" << GetName ()  << ")";
        mImplementation->join ();
    }
    else{
        LOGTRACE << "[Thread](" << GetName () << ") cannot be joined!";
    }
}