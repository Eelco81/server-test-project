
#include "SimHeatEquation.h"
#include <cmath>

using namespace SIM::COM; 

HeatEquation::HeatEquation (const std::string& inName) :
    SIM::Block (inName),
    mX0 (0.0),
    mXN (0.0),
    mL (1.0),
    mAlpha (0.001)
{
    AddPort (mSystem.OutputRef(), Path::OUTPUT, "x");
    AddPort (mU, Path::INPUT, "u");
    AddPort (&mX0, Path::PARAMETER, "x0");
    AddPort (&mXN, Path::PARAMETER, "xN");
    AddPort (&mL, Path::PARAMETER, "L");
    AddPort (&mAlpha, Path::PARAMETER, "alpha");
}

HeatEquation::~HeatEquation () = default;

void HeatEquation::Initialize (double inTime, double inTimeStep) {
    
    const auto dx = mL / static_cast<double> (N + 1u);
    
    const auto A = MATH::Diag<N> (-2.0 * mAlpha / dx / dx,  0) + 
                   MATH::Diag<N> ( 1.0 * mAlpha / dx / dx, -1) + 
                   MATH::Diag<N> ( 1.0 * mAlpha / dx / dx,  1);

    mSystem.Reset (A, MATH::Identity<N> (), MATH::Identity<N> (), 0.0, inTimeStep);
}

void HeatEquation::Step (double inTime, double inTimeStep) {
    
    const auto dx = mL / static_cast<double> (N + 1u);
    
    auto input (mU);
    input(0) += (mAlpha / dx / dx) * mX0;
    input(N-1) += (mAlpha / dx / dx) * mXN;
    
    mSystem.Update (input);
}

void HeatEquation::Terminate (double inTime, double inTimeStep) {
    Step (inTime, inTimeStep);
}
