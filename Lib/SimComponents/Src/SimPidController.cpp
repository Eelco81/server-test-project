
#include "SimPidController.h"
#include <cmath>

using namespace SIM::COM; 

PIDController::PIDController (const std::string& inName) :
    SIM::Block (inName),
    mKp (0.0),
    mKi (0.0),
    mKd (0.0),
    mInput (0.0),
    mOutput (0.0)
{
    AddInPort (&mInput, "input");
    AddInPort (&mSetpoint, "setpoint");
    AddOutPort (&mOutput, "output");
    AddParPort (&mKp, "kp");
    AddParPort (&mKi, "ki");
    AddParPort (&mKd, "kd");
}

PIDController::~PIDController () = default;

void PIDController::Initialize (double inTime) {
    mIntegral = 0.0;
    mPrevError = 0.0;
}

void PIDController::Step (double inTime) {
    double error (mSetpoint - mInput);
    double derivative ((error - mPrevError) / 0.1);
    mIntegral += 0.1 * error;
    mOutput = mKp * error + mKi * mIntegral + mKd * derivative;
    mPrevError = error;
}

void PIDController::Terminate (double inTime) {
    Step (inTime);
}

