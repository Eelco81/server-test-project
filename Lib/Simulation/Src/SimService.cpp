
#include <exception>
#include "Log.h"
#include "TriggerThread.h"
#include "SimLoop.h"
#include "SimFactory.h"
#include "SimService.h"

SIM::Service::Service (std::unique_ptr<Factory> inFactory) :
    mFactory (std::move (inFactory)),
    mRunner (nullptr),
    mLoop (nullptr),
    mIsLoaded (false),
    mIsRunning (false)
{
}

SIM::Service::~Service () = default;

bool SIM::Service::Load (const json& inConfig) {
    
    OS::SingleLock lock (mMutex);
    
    if (mIsRunning) {
        LOGMESSAGE (OS::Log::kWarning, "Cannot load a running simulation");
        return false;
    }
    
    try {
        mLoop = mFactory->Create (inConfig);
        mRunner = std::make_unique<APP::TriggerThread<Service>> ("SimulationRunner", mLoop->GetTimeStep (), this, &Service::Trigger);
    }
    catch (std::exception& e) {
        LOGMESSAGE (OS::Log::kError, e.what ());
        mLoop.reset (nullptr);
        mRunner.reset (nullptr);
        mIsLoaded = false;
        return false;
    }

    mIsLoaded = true;
    return true;
}

bool SIM::Service::Start () {
    
    OS::SingleLock lock (mMutex);
    
    if (!mIsLoaded) {
        LOGMESSAGE (OS::Log::kWarning, "Cannot start an unloaded simulation");
        return false;
    }
    
    if (mIsRunning) {
        LOGMESSAGE (OS::Log::kWarning, "Cannot restart a running simulation");
        return false;
    }
    
    if (mRunner) {
        mLoop->Initialize ();
        mRunner->Spawn ();
    }
    
    return true;
}

bool SIM::Service::Stop () {
    
    OS::SingleLock lock (mMutex);
    
    if (!mIsLoaded) {
        LOGMESSAGE (OS::Log::kWarning, "Cannot stop an unloaded simulation");
        return false;
    }
    
    if (mRunner) {
        mRunner->Kill ();
        mRunner->Join ();
        mLoop->Terminate ();
        mRunner.reset (nullptr);
        mLoop.reset (nullptr);
    }
    
    return true;
}

bool SIM::Service::GetValue (const std::string& inPath, std::string& outValue) {
    
    try {
        OS::SingleLock lock (mMutex);
        outValue = mLoop->GetValue (inPath);
        return true;
    }
    catch (std::exception& e) {
        LOGMESSAGE (OS::Log::kError, e.what ());
        return false;
    }
}

bool SIM::Service::Trigger () {
    
    try {
        OS::SingleLock lock (mMutex);
        mLoop->Update ();
        return true;
    }
    catch (std::exception& e) {
        LOGMESSAGE (OS::Log::kError, e.what ());
        return false;
    }
}

bool SIM::Service::IsLoaded () const {
    return mIsLoaded;
}

bool SIM::Service::IsRunning () const {
    return mIsRunning;
}
