

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
    virtual bool IsLoaded () const  = 0;
    virtual bool Start () = 0;
    virtual bool Stop ()  = 0;
    virtual bool IsRunning () const = 0;
    virtual std::vector<std::string> GetPaths () const = 0;
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
    std::vector<std::string> GetPaths () const override;
    
    /**
     * Trigger call of the service.
     */
    bool Trigger ();
    
private:
    std::unique_ptr<Factory> mFactory;
    std::unique_ptr<APP::PeriodicThread> mRunner;
    std::unique_ptr<Loop> mLoop;
    OS::Mutex mMutex;

};

} // end namespace SIM

#endif // _SIM_SERVICE_H_
