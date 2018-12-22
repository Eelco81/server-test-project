
#ifndef _SIM_HEATEQUATION_H_
#define _SIM_HEATEQUATION_H_

#include "SimBlock.h"
#include "MathVector.h"
#include "MathDynamicalSystem.h"

namespace SIM { namespace COM {

class HeatEquation : public SIM::Block {

public:
    static constexpr std::size_t N = 10u;

public:
    HeatEquation (const std::string& inName);
    ~HeatEquation ();
    
    void Initialize (double inTime, double inTimeStep) override;
    void Step (double inTime, double inTimeStep) override;
    void Terminate (double inTime, double inTimeStep) override;
    
private:
    MATH::Vector<N> mU;
    MATH::DynSys<N,N,N> mSystem;
    
    // double mX0;
    // double mXN;
    double mL;
    double mAlpha;

};

}}

#endif // _SIM_HEATEQUATION_H_
