
#include "SimWaveEquation.h"
#include <cmath>

using namespace SIM::COM; 

WaveEquation::WaveEquation (const std::string& inName) :
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

WaveEquation::~WaveEquation () = default;

void WaveEquation::Initialize (double inTime, double inTimeStep) {
    
    /* 
        x  = [ x(t), x'(t) ]
        
        x' = [ x'(t), x''(t)] = [ 0, I; A, 0 ] x + [ 0; I ] u
        y  = [ x(t) ] = [ I, 0 ] x + [ 0 ] * u
    */
    
    const auto dx = mL / static_cast<double> (N + 1u);
    
    const auto A = MATH::Diag<N> (-2.0 * mAlpha / dx / dx,  0) + 
                   MATH::Diag<N> ( 1.0 * mAlpha / dx / dx, -1) + 
                   MATH::Diag<N> ( 1.0 * mAlpha / dx / dx,  1);
    
    const auto Au = MATH::Augment<N> (MATH::Matrix<N,N> (0.0), MATH::Diag<N> (1.0, 0), A, MATH::Matrix<N,N> (0.0));
    
    MATH::Matrix<N*2,N> Bu;
    for (std::size_t i=0; i < N; i++) Bu(i+N,i) = 1.0;
    
    MATH::Vector<N*2> x0;
    for (std::size_t i=0; i < N; i++) x0(i+N) = std::sin((double)(i+1) * dx * 3.14);
    
    mSystem.Reset (Au, Bu, MATH::Diag<N*2> (1.0, 0), 0.0, inTimeStep, x0);
}

void WaveEquation::Step (double inTime, double inTimeStep) {
    
    const auto dx = mL / static_cast<double> (N + 1u);
    
    MATH::Vector<N> input;
    input(0) += (mAlpha / dx / dx) * mX0;
    input(N-1) += (mAlpha / dx / dx) * mXN;
    
    mSystem.Update (input);
}

void WaveEquation::Terminate (double inTime, double inTimeStep) {
    Step (inTime, inTimeStep);
}
