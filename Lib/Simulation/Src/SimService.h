

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

class IService {
public: 
    virtual bool Load (const json& inConfig) = 0;
    virtual bool UnLoad () = 0;
    virtual bool IsLoaded () const  = 0;
    virtual bool Start () = 0;
    virtual bool Stop ()  = 0;
    virtual bool IsRunning () const = 0;
};

class Service : public IService {

    NO_COPY_CONSTRUCTORS (Service);
    
public:
    Service (std::unique_ptr<Factory> inFactory);
    virtual ~Service ();
    
public:
    bool Load (const json& inConfig) override;
    bool UnLoad () override;
    bool IsLoaded () const override;
    
    bool Start () override;
    bool Stop () override;
    bool IsRunning () const override;

    bool GetValue (const std::string& inPath, std::string& outValue);

public:
    bool Trigger ();
    
private:
    std::unique_ptr<Factory> mFactory;
    std::unique_ptr<APP::PeriodicThread> mRunner;
    std::unique_ptr<Loop> mLoop;
    OS::Mutex mMutex;
};

}

#endif // _SIM_SERVICE_H_
