
#include "SimPidController.h"
#include <cmath>

using namespace SIM::COM; 

PIDController::PIDController (const std::string& inName) :
    SIM::Block (inName),
    mInput (0.0),
    mSetpoint (0.0),
    mOutput (0.0),
    mIntegral (0.0),
    mPrevError (0.0),
    mKp (0.0),
    mKi (0.0),
    mKd (0.0),
    mEnable (true)
{
    AddPort (&mInput, Path::INPUT, "input");
    AddPort (&mSetpoint, Path::INPUT, "setpoint");
    AddPort (&mOutput, Path::OUTPUT, "output");
    AddPort (&mKp, Path::PARAMETER, "kp");
    AddPort (&mKi, Path::PARAMETER, "ki");
    AddPort (&mKd, Path::PARAMETER, "kd");
    AddPort (&mEnable, Path::PARAMETER, "enable");
}

PIDController::~PIDController () = default;

void PIDController::Initialize (double inTime, double inTimeStep) {
    mIntegral = 0.0;
    mPrevError = 0.0;
    mOutput = 0.0;
}

void PIDController::Step (double inTime, double inTimeStep) {
    if (mEnable) {
        const double error (mSetpoint - mInput);
        const double derivative ((error - mPrevError) / inTimeStep);
        mPrevError = error;
        mIntegral += inTimeStep * error;
        mOutput = mKp * error + mKi * mIntegral + mKd * derivative;
    }
    else {
        mIntegral = 0.0;
        mPrevError = 0.0;
        mOutput = 0.0;
    }
}

void PIDController::Terminate (double inTime, double inTimeStep) {
    Step (inTime, inTimeStep);
}
