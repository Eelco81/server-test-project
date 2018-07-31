
#include "Log.h"
#include "Thread.h"

OS::Thread::Thread (const std::string& inName) :
    mName (inName),
    mStatus (kUnstarted)
{
}

OS::Thread ::~Thread () {
    // LOGMESSAGE (OS::Log::kDebug, std::string ("[Thread](") + GetName ()  + ") is being destructed");
}

void OS::Thread::Run () {
    
    LOGMESSAGE (OS::Log::kDebug, std::string ("[Thread](") + GetName () + ") started");
    mStatus = kRunning;
    
    Execute ();
    
    LOGMESSAGE (OS::Log::kDebug, std::string ("[Thread](") + GetName () + ") stopped");
    mStatus = kDone;
}

void OS::Thread::Spawn () {
    mImplementation.reset (new std::thread ([](Thread* inThread) { inThread->Run(); }, this));
}

void OS::Thread::Join () {
    if (mImplementation && mImplementation->joinable ()) {
        LOGMESSAGE (OS::Log::kTrace, std::string ("Joining [Thread](") + GetName ()  + ")");
        mImplementation->join ();
    }
    else{
        LOGMESSAGE (OS::Log::kTrace, std::string ("[Thread](") + GetName ()  + ") cannot be joined!");
    }
}