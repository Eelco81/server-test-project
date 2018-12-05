
#include "SimException.h"
#include "SimInitializer.h"

SIM::Initializer::Initializer (std::weak_ptr<Port> inPort, double inValue) :
    mPort (inPort),
    mValue (inValue)
{
}

SIM::Initializer::~Initializer () = default;

void SIM::Initializer::Apply () {
    auto port (mPort.lock ());
    if (!port) {
        throw SIM::Exception ("Port no longer exists");
    }
    port->SetNumericValue (mValue);
}
