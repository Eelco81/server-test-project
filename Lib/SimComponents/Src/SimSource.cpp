
#include "SimSource.h"
#include <cmath>
#include "Log.h"

#define _USE_MATH_DEFINES
#include <cmath>

using namespace SIM::COM; 

Source::Source (const std::string& inName) :
    SIM::Block (inName),
    mAmplitude (1.0),
    mPhase (0.0),
    mFrequency (M_PI),
    mOffset (0.0),
    mType (1u)
{
    AddOutPort (&mOutput, "value");
    AddParPort (&mAmplitude, "amplitude");
    AddParPort (&mPhase, "phase");
    AddParPort (&mFrequency, "frequency");
    AddParPort (&mOffset, "offset");
    AddParPort (&mType, "type");
}

Source::~Source () = default;

void Source::Initialize (double inTime) {
    Step (inTime);
}

void Source::Step (double inTime) {
    switch (mType) {
        case 0u:
            mOutput = mOffset; break;
        case 1u:
            mOutput = mOffset + mAmplitude * std::sin (mPhase + mFrequency * inTime); break;
        case 2u:
            mOutput = mOffset + mAmplitude * std::cos (mPhase + mFrequency * inTime); break;
        case 3u:
            mOutput = mOffset + mAmplitude * (std::sin (mPhase + mFrequency * inTime) > 0.0 ? 1.0: -1.0); break;
        default:
            mOutput = 0u;
    }
}

void Source::Terminate (double inTime) {
    Step (inTime);
}
