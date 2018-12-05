
#ifndef _SIM_INITIALIZER_H_
#define _SIM_INITIALIZER_H_

#include <memory>
#include "SimTypedPort.h"

namespace SIM {

class Initializer {
    
public:
    /**
     * Constructor.
     */
    Initializer (std::weak_ptr<Port> inPort, double inValue);
    
    /**
     * Virtual destructor.
     */
    virtual ~Initializer ();
    
    /**
     * Apply the value to the port.
     */
    void Apply ();
    
private:
    std::weak_ptr<Port> mPort;
    double mValue;
};

}

#endif // _SIM_INITIALIZER_H_
