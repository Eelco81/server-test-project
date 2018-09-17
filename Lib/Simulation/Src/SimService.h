

#ifndef _SIM_SERVICE_H_
#define _SIM_SERVICE_H_

#include "Macros.h"
#include "Mutex.h"
#include <atomic>
#include <memory>

#include <json.hpp>
using json = nlohmann::json;

namespace APP {
class PeriodicThread;
}

namespace SIM {

class Loop;
class Factory;

class Service {

    NO_COPY_CONSTRUCTORS (Service);
    
public:
    Service (std::unique_ptr<Factory> inFactory);
    virtual ~Service ();
    
public:
    bool Load (const json& inConfig);
    bool UnLoad ();
    bool IsLoaded () const;
    
    bool Start ();
    bool Stop ();
    bool IsRunning () const;

    bool GetValue (const std::string& inPath, std::string& outValue);

public:
    bool Trigger ();
    
private:
    std::unique_ptr<Factory> mFactory;
    std::unique_ptr<APP::PeriodicThread> mRunner;
    std::unique_ptr<Loop> mLoop;
    std::atomic<bool> mIsLoaded;
    std::atomic<bool> mIsRunning;
    OS::Mutex mMutex;
};

}

#endif // _SIM_SERVICE_H_
