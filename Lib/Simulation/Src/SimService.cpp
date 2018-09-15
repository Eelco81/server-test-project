
#include <exception>
#include "Log.h"
#include "PeriodicThread.h"
#include "Task.h"
#include "SimLoop.h"
#include "SimFactory.h"
#include "SimService.h"

namespace {

class SimulationTask : public APP::Task {
public:
    SimulationTask (SIM::Service& inService) :
        APP::Task ("SimulationTask"),
        mService (inService)
    {
    }
    virtual bool Step () override {
        return mService.Trigger ();
    }
protected:
    SIM::Service& mService;
};

class SimulationRunner : public APP::PeriodicThread {
public: 
    SimulationRunner (uint64_t inWaitTime, SIM::Service& inService) :
        APP::PeriodicThread ("SimulationRunner", inWaitTime)
    {
        AddTask (std::make_unique<SimulationTask> (inService));
    }
};

}

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
    
    if (mIsRunning) {
        LOGMESSAGE (OS::Log::kWarning, "Cannot load a running simulation");
        return false;
    }
    
    try {
        mLoop = mFactory->Create (inConfig);
        mRunner = std::make_unique<SimulationRunner> (100u, *this);
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

bool SIM::Service::Trigger () {
    try {
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