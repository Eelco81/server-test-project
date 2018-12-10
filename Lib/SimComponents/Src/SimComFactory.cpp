
#include "SimComFactory.h"
#include "SimSource.h"
#include "SimOscillator.h"
#include "SimPidController.h"

std::unique_ptr<SIM::Block> SIM::COM::Factory::CreateBlock (const std::string& inName, const std::string& inType) {
    
    if (inType == "source") {
        return std::make_unique <SIM::COM::Source> (inName);
    }
    if (inType == "oscillator") {
        return std::make_unique <SIM::COM::Oscillator> (inName);
    }
    if (inType == "pid") {
        return std::make_unique <SIM::COM::PIDController> (inName);
    }
    return std::unique_ptr <SIM::Block> ();
};

