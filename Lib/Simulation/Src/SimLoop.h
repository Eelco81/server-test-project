
#ifndef _SIM_LOOP_H_
#define _SIM_LOOP_H_

#include <memory>
#include <vector>
#include <string>

#include "SimPath.h"
#include "SimTimer.h"
#include "SimSampler.h"

namespace SIM {

class Port;
class Block;
class Connector;

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
     * Add to sampler
     */
    void AddSample (const std::string& inSource);
    
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
     * Get a value of a path inside the loop
     */
    std::string GetValue (const std::string& inPath) const;

    /**
     * Get all paths
     */
    std::vector<Path> GetPaths () const;
    
    /**
     * Get the current timestamp
     */
    uint64_t GetTimeStamp () const;
    
    /**
     * Get the loops time step
     */
    uint64_t GetTimeStep () const;

    /**
     * Get the sampler stream
     */
    inline Sampler& GetSampler () { return mSampler; }
    
private:
    /**
     * Find a port based on a path inside the loop
     */
    std::weak_ptr<Port> FindPort (const Path& inPath) const;
    
private:
    Timer mTimer;
    Sampler mSampler;
    std::vector<std::unique_ptr<Block>> mBlocks;
    std::vector<std::unique_ptr<Connector>> mConnectors;
    
};

}

#endif // _SIM_LOOP_H_
