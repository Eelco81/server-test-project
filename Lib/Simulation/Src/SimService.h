
#ifndef _SIM_SERVICE_H_
#define _SIM_SERVICE_H_

#include <atomic>
#include <memory>

#include "MessageStream.h"
#include "Macros.h"
#include "Mutex.h"

#include "SimPath.h"
#include "SimSampleStream.h"

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
    virtual bool IsLoaded () const  = 0;
    virtual bool Start () = 0;
    virtual bool Stop ()  = 0;
    virtual bool IsRunning () const = 0;
    virtual std::vector<Path> GetPaths () const = 0;
};

class Service : public IService {
    
    NO_COPY_CONSTRUCTORS (Service);
    
public:
    /**
     * Constructor
     */
    Service (std::unique_ptr<Factory> inFactory);
    
    /**
     * Destructor
     */
    virtual ~Service ();
    
    /**
     * Load the simulation service.
     */
    bool Load (const json& inConfig) override;
    
    /**
     * Verify if thhe simulation service is loaded.
     */
    bool IsLoaded () const override;
    
    /**
     * Start the simulation.
     */
    bool Start () override;
    
    /**
     * Stop the simulation.
     */
    bool Stop () override;
    
    /**
     * Verify if thhe simulation service is running.
     */
    bool IsRunning () const override;
    
    /**
     * Retrieve value from the running simulation.
     */
    bool GetValue (const std::string& inPath, std::string& outValue);
    
    /**
     * Get all valid paths inside the simulation
     */
    std::vector<Path> GetPaths () const override;
    
    /**
     * Trigger call of the service.
     */
    bool Trigger ();
    
    /**
     * Get sample stream
     */
    inline SampleStream& GetSampleStream () { return mSampleStream; }
    
private:
    std::unique_ptr<Factory> mFactory;
    std::unique_ptr<APP::PeriodicThread> mRunner;
    std::unique_ptr<Loop> mLoop;
    OS::Mutex mMutex;
    SampleStream mSampleStream;
};

} // end namespace SIM

#endif // _SIM_SERVICE_H_
