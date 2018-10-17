
#include "SimSource.h"
#include "Math.h"
#include "Log.h"

#define _USE_MATH_DEFINES
#include <cmath>

using namespace SIM::COM; 

Source::Source (const std::string& inName) :
    SIM::Block (inName),
    mAmplitude (1.0),
    mPhase (0.0),
    mFrequency (M_PI),
    mOffset (0.0)
{
    AddOutPort (&mOutput, "value");
    AddParPort (&mAmplitude, "amplitude");
    AddParPort (&mPhase, "phase");
    AddParPort (&mFrequency, "frequency");
    AddParPort (&mOffset, "offset");
}

Source::~Source () = default;

void Source::Initialize (double inTime) {
    Step (inTime);
}

void Source::Step (double inTime) {
    mOutput = mOffset + mAmplitude * std::sin (mPhase + mFrequency * inTime);
    // LOGINFO << "source " << inTime << " " << mOutput;
}

void Source::Terminate (double inTime) {
    Step (inTime);
}
