
#ifndef _SIM_WAVEEQUATION_H_
#define _SIM_WAVEEQUATION_H_

#include "SimBlock.h"
#include "MathVector.h"
#include "MathDynamicalSystem.h"

namespace SIM { namespace COM {

class WaveEquation : public SIM::Block {

public:
    static constexpr std::size_t N = 50u;

public:
    WaveEquation (const std::string& inName);
    ~WaveEquation ();
    
    void Initialize (double inTime, double inTimeStep) override;
    void Step (double inTime, double inTimeStep) override;
    void Terminate (double inTime, double inTimeStep) override;
    
private:
    MATH::Vector<N> mU;
    MATH::DynSys<N*2,N,N*2> mSystem;
    
    double mX0;
    double mXN;
    double mL;
    double mAlpha;

};

}}

#endif // _SIM_WAVEEQUATION_H_
