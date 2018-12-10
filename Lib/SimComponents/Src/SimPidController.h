
#ifndef _SIM_PIDCONTROLLER_H_
#define _SIM_PIDCONTROLLER_H_

#include "SimBlock.h"

namespace SIM { namespace COM {

class PIDController : public SIM::Block {

public:
    PIDController (const std::string& inName);
    ~PIDController ();
    
    void Initialize (double inTime) override;
    void Step (double inTime) override;
    void Terminate (double inTime) override;
    
private:

    double mInput;
    double mSetpoint;
    double mOutput;
    double mIntegral;
    double mPrevError;
    double mKp;
    double mKi;
    double mKd;
};

}}

#endif // _SIM_PIDCONTROLLER_H_
