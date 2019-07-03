
#define _USE_MATH_DEFINES
#include <cmath>

#include "SimSource.h"
#include "Log.h"

using namespace SIM::COM; 

Source::Source (const std::string& inName) :
    SIM::Block (inName),
    mAmplitude (1.0),
    mPhase (0.0),
    mFrequency (M_PI),
    mOffset (0.0),
    mType (1u)
{
    AddPort (&mOutput, Path::OUTPUT, "value");
    AddPort (&mAmplitude, Path::PARAMETER, "amplitude");
    AddPort (&mPhase, Path::PARAMETER, "phase");
    AddPort (&mFrequency, Path::PARAMETER, "frequency");
    AddPort (&mOffset, Path::PARAMETER, "offset");
    AddPort (&mType, Path::PARAMETER, "type");
}

Source::~Source () = default;

void Source::Initialize (double inTime, double inTimeStep) {
    Step (inTime, inTimeStep);
}

void Source::Step (double inTime, double inTimeStep) {
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

void Source::Terminate (double inTime, double inTimeStep) {
    Step (inTime, inTimeStep);
}
