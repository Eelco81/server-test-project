
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

/*
template <typename T>
class TypedConnector : public Connector {

public:
    TypedConnector (std::weak_ptr<TypedPort<T>> inSource, std::weak_ptr<TypedPort<T>> inTarget) :
        Connector (),
        mSource (inSource),
        mTarget (inTarget)
    {
    }
    virtual ~TypedConnector () = default;
    
    virtual void Transfer () override {
        auto source (mSource.lock ());
        auto target (mTarget.lock ());
        if (source && target) {
            *target = *source;
        }
    }

protected:
    std::weak_ptr<TypedPort<T>> mSource;
    std::weak_ptr<TypedPort<T>> mTarget;
};
*/
}

#endif // _SIM_CONNECTOR_H_
