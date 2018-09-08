
#include "SimService.h"

SIM::Service::Service () :
    mIsLoaded (false),
    mIsRunning (false)
{
}

SIM::Service::~Service () = default;

bool SIM::Service::Load () {
    return false;
}

bool SIM::Service::UnLoad () {
    return false;
}

bool SIM::Service::Start () {
    return false;
}

bool SIM::Service::Stop () {
    return false;
}

bool SIM::Service::IsLoaded () const {
    return mIsLoaded;
}

bool SIM::Service::IsRunning () const {
    return mIsRunning;
}