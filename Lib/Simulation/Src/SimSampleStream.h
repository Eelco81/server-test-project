
#ifndef _SIM_SAMPLESTREAM_H_
#define _SIM_SAMPLESTREAM_H_

#include <cstdint>
#include <vector>
#include <memory>

#include "MessageStream.h"
#include "SimEvent.h"

namespace SIM {

class EventStream : public OS::MessageStream<Event, std::string> {

public:
    /**
     * Constructor
     */
    EventStream ();
    
    /**
     * Virtual destructor
     */
    virtual ~EventStream ();
    
    /**
     * Forward the sample data
     */
    void Write (const Event& inEvent) override;
    
};

}

#endif // _SIM_SAMPLESTREAM_H_
