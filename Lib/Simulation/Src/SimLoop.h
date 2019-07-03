
#ifndef _SIM_LOOP_H_
#define _SIM_LOOP_H_

#include <memory>
#include <vector>
#include <string>

#include "MessageStream.h"
#include "SimValue.h"
#include "SimPath.h"
#include "SimTimer.h"
#include "SimSampler.h"
#include "SimEvent.h"

namespace SIM {

class Port;
class Block;
class Connector;
class Initializer;

class Loop {

public:
    /**
     * Constructor
     */
    Loop (uint64_t inTimeStamp);
    
    /**
     * Virtual destructor
     */
    virtual ~Loop ();

    /**
     * Add a block to the loop
     */
    void AddBlock (std::unique_ptr<Block> inBlock);
    
    /**
     * Connect an output path to an input path inside the loop
     */
    void Connect (const std::string& inSource, const std::string& inTarget);
    
    /**
     * Connect an output path to an input path inside the loop
     */
    void SetInitializer (const std::string& inPort, double inValue);
    
    /**
     * Add sampler
     */
    void AddSampler (const std::vector<std::string>& inPaths, Sampler::Type inType);
    
    /**
     * Initialize the loop
     */
    void Initialize ();
    
    /**
     * Do a step update of the loop
     */
    void Update ();
    
    /**
     * Terminate the loop
     */
    void Terminate ();
    
    /**
     * Get a value of a all paths inside the loop
     */
    std::vector<Value> GetValues () const;

    /**
     * Get a value of a path inside the loop
     */
    Value GetValue (const Path& inPath) const;

    /**
     * Set a value of inside the loop
     */
    void SetValue (const Value& inValue);

    /**
     * Get all paths
     */
    std::vector<Path> GetPaths () const;
    
    /**
     * Get sampler infos
     */
    std::vector<Sampler::Info> GetSamplers () const;
    
    /**
     * Get the current timestamp
     */
    uint64_t GetTimeStamp () const;
    
    /**
     * Get the loops time step
     */
    uint64_t GetTimeStep () const;
    
    /**
     * Emits Events from within the loop.
     */
    OS::Signal<const Event&> sEventAvailable;
    
private:
    /**
     * Find a port based on a path inside the loop
     */
    std::weak_ptr<Port> FindPort (const Path& inPath) const;
    
private:
    Timer mTimer;
    std::vector<std::unique_ptr<Block>> mBlocks;
    std::vector<std::unique_ptr<Connector>> mConnectors;
    std::vector<std::unique_ptr<Initializer>> mInitializers;
    std::vector<std::unique_ptr<Sampler>> mSamplers;
    
};

}

#endif // _SIM_LOOP_H_
