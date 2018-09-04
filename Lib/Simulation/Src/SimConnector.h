
#ifndef _SIM_CONNECTOR_H_
#define _SIM_CONNECTOR_H_

#include <memory>
#include "SimTypedPort.h"

namespace SIM {

class Connector {
    
public:
    Connector (std::weak_ptr<Port> inSource, std::weak_ptr<Port> inTarget);
    virtual ~Connector ();
    
public:
    void Transfer ();
    
protected:
    class Implementation;
    std::unique_ptr<Implementation> mImpl;

};

}

#endif // _SIM_CONNECTOR_H_
