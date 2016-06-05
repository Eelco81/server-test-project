
#include "Log.h"
#include "Thread.h"

namespace {
    void LaunchThreadExecution (OS::Thread* inThread) {
        inThread->Run ();
    }
}

OS::Thread::Thread (const std::string& inName) :
    mName (inName),
    mStatus (kUnstarted)
{
}

OS::Thread ::~Thread () {
    LOGMESSAGE (OS::Log::kDebug, std::string ("[Thread](") + GetName ()  + ") is being destructed");
}

void OS::Thread::Run () {
    mStatus = kRunning;
    Execute ();
    mStatus = kDone;
}

void OS::Thread::Spawn () {
    LOGMESSAGE (OS::Log::kDebug, std::string ("[Thread](") + GetName () + ") started");
    mImplementation.reset (new std::thread (LaunchThreadExecution, this));
}

void OS::Thread::Join () {
    if (mImplementation && mImplementation->joinable ()) {
        mImplementation->join ();
    }
}