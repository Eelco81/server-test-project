
#ifndef _SIM_OSCILATOR_H_
#define _SIM_OSCILATOR_H_

#include "SimBlock.h"
#include "MathDynamicalSystem.h"

namespace SIM { namespace COM {

class Oscillator : public SIM::Block {

public:
    Oscillator (const std::string& inName);
    ~Oscillator ();

    void Initialize (double inTime) override;
    void Step (double inTime) override;
    void Terminate (double inTime) override;
    
private:
    double mForce;
    double mVelocity;
    double mPosition;
    double mMass;
    double mDamping;
    double mFriction;
    
    double mPrevTime;
    MATH::DynSys<2,1,3,double> mSystem;

};

} } // end namespace SIM::COM


#endif // _SIM_OSCILATOR_H_
