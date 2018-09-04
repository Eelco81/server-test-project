

#include "SimException.h"

SIM::Exception::Exception (const std::string& inMessage) :
    mMessage (inMessage)
{
}

SIM::Exception::~Exception () = default;

const char* SIM::Exception::what () const throw () {
    return mMessage.c_str ();
}
