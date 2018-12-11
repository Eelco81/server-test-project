
#ifndef _SIM_PIDCONTROLLER_H_
#define _SIM_PIDCONTROLLER_H_

#include "SimBlock.h"

namespace SIM { namespace COM {

class PIDController : public SIM::Block {

public:
    PIDController (const std::string& inName);
    ~PIDController ();
    
    void Initialize (double inTime, double inTimeStep) override;
    void Step (double inTime, double inTimeStep) override;
    void Terminate (double inTime, double inTimeStep) override;
    
private:
    double mInput;
    double mSetpoint;
    double mOutput;
    double mIntegral;
    double mPrevError;
    double mKp;
    double mKi;
    double mKd;
    bool mEnable;
};

}}

#endif // _SIM_PIDCONTROLLER_H_
