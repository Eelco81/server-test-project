
#include <exception>
#include "Log.h"
#include "TriggerThread.h"
#include "SimLoop.h"
#include "SimFactory.h"
#include "SimService.h"

SIM::Service::Service (std::unique_ptr<Factory> inFactory) :
    mFactory (std::move (inFactory)),
    mRunner (nullptr),
    mLoop (nullptr)
{
}

SIM::Service::~Service () = default;

bool SIM::Service::Load (const json& inConfig) {
    
    OS::SingleLock lock (mMutex);
    
    if (IsRunning ()) {
        LOGMESSAGE (OS::Log::kWarning, "Cannot load a running simulation");
        return false;
    }
    
    try {
        mLoop = mFactory->Create (inConfig);
        mLoop->GetSampler ().Pipe (mSampleStream);
    }
    catch (std::exception& e) {
        LOGMESSAGE (OS::Log::kError, e.what ());
        mLoop.reset (nullptr);
        mRunner.reset (nullptr);
        return false;
    }
    
    LOGMESSAGE (OS::Log::kInfo, "Loaded simulation...");
    
    return true;
}

bool SIM::Service::Start () {
    
    OS::SingleLock lock (mMutex);
    
    if (!IsLoaded ()) {
        LOGMESSAGE (OS::Log::kWarning, "Cannot start an unloaded simulation");
        return false;
    }
    
    if (IsRunning ()) {
        LOGMESSAGE (OS::Log::kWarning, "Cannot restart a running simulation");
        return false;
    }
    
    mRunner = std::make_unique<APP::TriggerThread<Service>> ("SimulationRunner", mLoop->GetTimeStep (), this, &Service::Trigger);
    
    mLoop->Initialize ();
    mRunner->Spawn ();
    
    LOGMESSAGE (OS::Log::kInfo, "Initialized loop, starting simulation.");
    return true;
}

bool SIM::Service::Stop () {
    
    OS::SingleLock lock (mMutex);
    
    if (!IsLoaded ()) {
        LOGMESSAGE (OS::Log::kWarning, "Cannot stop an unloaded simulation");
        return false;
    }
    
    if (!IsRunning ()) {
        LOGMESSAGE (OS::Log::kWarning, "Cannot stop a stopped simulation");
        return false;
    }
    
    mRunner.reset (nullptr);
    mLoop.reset (nullptr);
    
    LOGMESSAGE (OS::Log::kInfo, "Stopped simulation.");
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

std::vector<SIM::Path> SIM::Service::GetPaths () const {
    if (IsLoaded ()) {
        return mLoop->GetPaths ();
    }
    return {};
}

bool SIM::Service::Trigger () {
    
    try {
        OS::SingleLock lock (mMutex);
        mLoop->Update ();
        return true;
    }
    catch (std::exception& e) {
        LOGMESSAGE (OS::Log::kError, std::string ("Simulation killed: ") + e.what ());
        return false;
    }
}

bool SIM::Service::IsLoaded () const {
    return mLoop.get () != nullptr;
} 

bool SIM::Service::IsRunning () const {
    return mRunner.get () != nullptr;
}
