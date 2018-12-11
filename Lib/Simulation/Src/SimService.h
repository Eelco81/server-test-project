
#ifndef _SIM_SERVICE_H_
#define _SIM_SERVICE_H_

#include <atomic>
#include <memory>

#include "MessageStream.h"
#include "Macros.h"
#include "Mutex.h"

#include "SimValue.h"
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
    virtual void Load (const json& inConfig) = 0;
    virtual bool IsLoaded () const  = 0;
    virtual void Start () = 0;
    virtual void Stop ()  = 0;
    virtual bool IsRunning () const = 0;
    virtual std::vector<Value> GetValues () = 0;
    virtual Value GetValue (const Path& inPath) = 0;
    virtual void SetValue (const Value& inValue) = 0;
    virtual std::vector<Path> GetPaths () = 0;
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
    void Load (const json& inConfig) override;
    
    /**
     * Verify if thhe simulation service is loaded.
     */
    bool IsLoaded () const override;
    
    /**
     * Start the simulation.
     */
    void Start () override;
    
    /**
     * Stop the simulation.
     */
    void Stop () override;
    
    /**
     * Verify if thhe simulation service is running.
     */
    bool IsRunning () const override;
    
    /**
     * Retrieve all values from the running simulation.
     */
    std::vector<Value> GetValues () override;
    
    /**
     * Retrieve value from the running simulation.
     */
    Value GetValue (const Path& inPath) override;
    
    /**
     * Set value from the running simulation.
     */
    void SetValue (const Value& inValue) override;
    
    /**
     * Get all valid paths inside the simulation
     */
    std::vector<Path> GetPaths () override;
    
    /**
     * Trigger call of the service.
     */
    bool Trigger ();
    
    /**
     * Get sample stream
     */
    inline EventStream& GetStream () { return mStream; }
    
private:
    std::unique_ptr<Factory> mFactory;
    std::unique_ptr<APP::PeriodicThread> mRunner;
    std::unique_ptr<Loop> mLoop;
    OS::Mutex mMutex;
    EventStream mStream;
};

} // end namespace SIM

#endif // _SIM_SERVICE_H_
