
#include "SimOscillator.h"
#include <cmath>

using namespace SIM::COM; 

Oscillator::Oscillator (const std::string& inName) :
    SIM::Block (inName),
    mForce (0.0),
    mControl (0.0),
    mMass (1.0),
    mDamping (1.0),
    mFriction (1.0)
{
    AddInPort (&mForce, "force");
    AddInPort (&mControl, "control");
    AddOutPort (&mSystem.OutputRef()(0), "position");
    AddOutPort (&mSystem.OutputRef()(1), "velocity");
    AddOutPort (&mSystem.OutputRef()(2), "acceleration");
    AddParPort (&mMass, "mass");
    AddParPort (&mDamping, "damping");
    AddParPort (&mFriction, "friction");
}

Oscillator::~Oscillator () = default;

void Oscillator::Initialize (double inTime, double inTimeStep) {
    
    MATH::Matrix<2,2> A (0.0);
    MATH::Matrix<2,1> B (0.0);
    MATH::Matrix<3,2> C (0.0);
    MATH::Matrix<3,1> D (0.0);
    
    /**
     * m * x'' + d * x' + k * x = F (t) 
     */
    
    A(0,1) = 1.0;
    A(1,0) = -mFriction / mMass;
    A(1,1) = -mDamping / mMass;
    B(1,0) = 1.0 / mMass;
    C(0,0) = 1.0;
    C(1,1) = 1.0;
    C(2,0) = -mFriction / mMass;
    C(2,1) = -mDamping / mMass;
    D(2,0) = 1.0 / mMass; 
    
    mSystem.Reset (A, B, C, D, inTimeStep);
}

void Oscillator::Step (double inTime, double inTimeStep) {
    mSystem.Update (mForce + mControl);
}

void Oscillator::Terminate (double inTime, double inTimeStep) {
    Step (inTime, inTimeStep);
}
