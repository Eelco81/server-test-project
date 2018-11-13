
#ifndef _SIM_SAMPLER_H_
#define _SIM_SAMPLER_H_

#include <cstdint>
#include <vector>
#include <memory>

#include "MessageStream.h"

namespace SIM {

class Port;

class Sampler : public OS::MessageStream<uint64_t,std::vector<double>> {

public:
    /**
     * Constructor
     */
    Sampler ();
    
    /**
     * Virtual destructor
     */
    virtual ~Sampler ();
    
    /**
     * Add port to the sampler
     */
    void AddPort (std::weak_ptr<Port> inPort);
    
    /**
     * Record the current list of ports
     */
    void Write (const uint64_t& inTimeStamp) override;
    
protected:
    std::vector<std::weak_ptr<Port>> mPorts;
    std::vector<double> mOutputs;

};

}

#endif // _SIM_SAMPLER_H_
