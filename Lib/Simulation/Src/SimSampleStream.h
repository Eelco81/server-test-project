
#ifndef _SIM_SAMPLESTREAM_H_
#define _SIM_SAMPLESTREAM_H_

#include <cstdint>
#include <vector>
#include <memory>

#include "MessageStream.h"

namespace SIM {

class SampleStream : public OS::MessageStream<std::vector<double>, std::string> {

public:
    /**
     * Constructor
     */
    SampleStream ();
    
    /**
     * Virtual destructor
     */
    virtual ~SampleStream ();
    
    /**
     * Forward the sample data
     */
    void Write (const std::vector<double>& inList) override;
    
};

}

#endif // _SIM_SAMPLESTREAM_H_
