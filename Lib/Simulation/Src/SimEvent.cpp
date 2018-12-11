
#include "SimEvent.h"

SIM::Event::Event (const std::string& inId) :
    mId (inId) 
{
}

SIM::Event::~Event () = default;

std::string SIM::Event::ToString () const {
    json j;
    j ["event-id"] = mId;
    j ["event-data"] = mData;
    return j.dump ();
}
