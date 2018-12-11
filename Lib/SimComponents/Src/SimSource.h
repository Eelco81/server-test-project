
#ifndef _SIM_SOURCE_H_
#define _SIM_SOURCE_H_

#include "SimBlock.h"

namespace SIM { namespace COM {

class Source : public SIM::Block {

public:
    Source (const std::string& inName);
    ~Source ();

    void Initialize (double inTime, double inTimeStep) override;
    void Step (double inTime, double inTimeStep) override;
    void Terminate (double inTime, double inTimeStep) override;
    
private:
    double mOutput;
    double mAmplitude;
    double mPhase;
    double mFrequency;
    double mOffset;
    uint8_t mType;

};

} } // end namespace SIM::COM

#endif // _SIM_SOURCE_H_
