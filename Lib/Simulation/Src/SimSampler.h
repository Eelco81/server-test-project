
#ifndef _SIM_SAMPLER_H_
#define _SIM_SAMPLER_H_

#include <cstdint>
#include <vector>
#include <memory>

#include "MessageStream.h"
#include "SimEvent.h"
#include "SimPort.h"

namespace SIM {

class Sampler : public OS::MessageStream<uint64_t,Event> {

public:
    
    /**
     * Sampler type
     */
    enum Type : uint8_t {
        HIST = 0x00,
        ROW
    };

    /**
     * Info 
     */
    struct Info {
        std::vector<Port::Info> mPortInfos;
        std::string mId;
        Type mType;
    };
    
    /**
     * Constructor
     */
    Sampler (std::size_t inId, Type inType = HIST);
    
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
    
    /**
     * Get sampler info
     */
    Info GetInfo () const;
    
protected:
    std::vector<std::weak_ptr<Port>> mPorts;
    std::vector<double> mOutputs;
    std::string mId;
    Type mType;

};

}

#endif // _SIM_SAMPLER_H_
