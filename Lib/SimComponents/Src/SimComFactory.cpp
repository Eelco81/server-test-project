
#include "SimComFactory.h"
#include "SimSource.h"
#include "SimOscillator.h"
#include "SimPidController.h"
#include "SimHeatEquation.h"
#include "SimWaveEquation.h"

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
    if (inType == "heat") {
        return std::make_unique <SIM::COM::HeatEquation> (inName);
    }
    if (inType == "wave") {
        return std::make_unique <SIM::COM::WaveEquation> (inName);
    }
    return std::unique_ptr <SIM::Block> ();
};

