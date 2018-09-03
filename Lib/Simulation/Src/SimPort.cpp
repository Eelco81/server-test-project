
#include "SimPort.h"

SIM::Port::Port (const std::string& inName) : 
    mName (inName) 
{
}

SIM::Port::~Port () = default;

const std::string& SIM::Port::GetName () const { 
    return mName; 
}
