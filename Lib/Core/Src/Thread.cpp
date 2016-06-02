
#include "Log.h"
#include "Thread.h"

namespace {
    void LaunchThreadExecution (Core::Thread* inThread) {
        inThread->Run ();
    }
}

Core::Thread::Thread (const std::string& inName) :
    mName (inName),
    mStatus (kUnstarted)
{
}

Core::Thread ::~Thread () {
    Core::Log::Instance ().LogMessage (Core::Log::kTrace, std::string ("[Thread](") + GetName ()  + ") is being destructed");
}

void Core::Thread::Run () {
    mStatus = kRunning;
    Execute ();
    mStatus = kDone;
}

void Core::Thread::Spawn () {
    Core::Log::Instance ().LogMessage (Core::Log::kTrace, std::string ("[Thread](") + GetName () + ") started");
    mImplementation.reset (new std::thread (LaunchThreadExecution, this));
}

void Core::Thread::Join () {
    if (mImplementation && mImplementation->joinable ()) {
        mImplementation->join ();
    }
}