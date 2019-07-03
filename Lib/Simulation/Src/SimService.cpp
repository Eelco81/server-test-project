
#include <exception>
#include "Log.h"
#include "TriggerThread.h"
#include "SimLoop.h"
#include "SimFactory.h"
#include "SimService.h"
#include "SimEvent.h"
#include "SimException.h"

SIM::Service::Service (std::unique_ptr<Factory> inFactory) :
    mFactory (std::move (inFactory)),
    mRunner (nullptr),
    mLoop (nullptr)
{
    sEventAvailable.Connect (this, &Service::EventToString);
}

SIM::Service::~Service () = default;

void SIM::Service::Load (const json& inConfig) {
    
    if (IsRunning ()) {
        throw Exception ("Cannot load a running simulation");
    }
    
    OS::SingleLock lock (mMutex);
    try {
        mLoop = mFactory->Create (inConfig);
        mLoop->sEventAvailable.Forward (sEventAvailable);
    }
    catch (Exception& e) {
        mLoop.reset (nullptr);
        mRunner.reset (nullptr);
        throw e;
    }
    
    sEventAvailable.Emit (Event ("sim-loaded"));
    LOGMESSAGE (OS::Log::kInfo, "Loaded simulation...");
}

void SIM::Service::Start () {
    
    if (!IsLoaded ()) {
        throw Exception ("Cannot start an unloaded simulation");
    }
    if (IsRunning ()) {
        throw Exception ("Cannot restart a running simulation");
    }
    
    LOGMESSAGE (OS::Log::kInfo, "Initializing loop, starting simulation.");
    
    OS::SingleLock lock (mMutex);
    mRunner = std::make_unique<APP::TriggerThread<Service>> ("SimulationRunner", mLoop->GetTimeStep (), this, &Service::Trigger);
    mLoop->Initialize ();
    mRunner->Spawn ();
    
    sEventAvailable.Emit (Event ("sim-started"));
}

void SIM::Service::Stop () {
    
    if (!IsLoaded ()) {
        throw Exception ("Cannot stop an unloaded simulation");
    }
    if (!IsRunning ()) {
        throw Exception ("Cannot stop a stopped simulation");
    }
    
    LOGMESSAGE (OS::Log::kInfo, "Stopped simulation.");
    
    OS::SingleLock lock (mMutex);
    mRunner.reset (nullptr);
    mLoop.reset (nullptr);
    
    sEventAvailable.Emit (Event ("sim-stopped"));
}

std::vector<SIM::Value> SIM::Service::GetValues () {
    
    if (!IsLoaded ()) {
        throw Exception ("Simulation not loaded");
    }
    
    OS::SingleLock lock (mMutex);
    return mLoop->GetValues ();
}

SIM::Value SIM::Service::GetValue (const SIM::Path& inPath) {
    
    if (!IsLoaded ()) {
        throw Exception ("Simulation not loaded");
    }
    
    OS::SingleLock lock (mMutex);
    return mLoop->GetValue (inPath);
}

void SIM::Service::SetValue (const SIM::Value& inValue) {
    
    if (!IsLoaded ()) {
        throw Exception ("Simulation not loaded");
    }
    
    OS::SingleLock lock (mMutex);
    return mLoop->SetValue (inValue);
}

std::vector<SIM::Path> SIM::Service::GetPaths () {
    
    if (!IsLoaded ()) {
        throw Exception ("Simulation not loaded");
    }
    
    OS::SingleLock lock (mMutex);
    return mLoop->GetPaths ();
}

std::vector<SIM::Sampler::Info> SIM::Service::GetSamplers () const {
    return mLoop->GetSamplers ();
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

void SIM::Service::EventToString (const SIM::Event& inEvent) {
    sEventStrAvailable.Emit (inEvent.ToString ());
}
