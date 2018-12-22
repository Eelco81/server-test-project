
#include "SimPort.h"

SIM::Port::Port (const SIM::Path& inPath) : 
    mPath (inPath) 
{
}

SIM::Port::~Port () = default;

const SIM::Path& SIM::Port::GetPath () const { 
    return mPath; 
}

SIM::Port::Info SIM::Port::GetInfo () const {
    return { GetPath (), GetType () };
}
