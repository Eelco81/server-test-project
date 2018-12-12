
#ifndef _SIM_CONNECTOR_H_
#define _SIM_CONNECTOR_H_

#include <memory>
#include "SimTypedPort.h"

namespace SIM {

class Connector {
    
public:
    /**
     * Constructor
     */
    Connector (std::weak_ptr<Port> inSource, std::weak_ptr<Port> inTarget);
    
    /**
     * Virtual destructor
     */
    virtual ~Connector ();
    
    /**
     * Copy value from source port to target port
     */
    void Transfer ();
    
protected:
    /**
     * PImple idiom
     */
    class Implementation;
    std::unique_ptr<Implementation> mImpl;

};

}

#endif // _SIM_CONNECTOR_H_
