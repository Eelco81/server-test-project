
#include "SimTimer.h"

SIM::Timer::Timer (uint64_t inTimeStep) :
    mTimeStep (inTimeStep),
    mTime (0u)
{
}

void SIM::Timer::Initialize () {
    mTime = 0u;
}

void SIM::Timer::Tick () {
    mTime += mTimeStep;
}

double SIM::Timer::GetTime () const {
    return static_cast<double> (mTime) / 1000000.0;
}

double SIM::Timer::GetTimeStepValue () const {
    return static_cast<double> (mTimeStep) / 1000000.0;
}

uint64_t SIM::Timer::GetTimeStamp () const {
    return mTime;
}

uint64_t SIM::Timer::GetTimeStep () const {
    return mTimeStep;
}
