
#include "SimComFactory.h"
#include "SimSource.h"

std::unique_ptr<SIM::Block> SIM::COM::Factory::CreateBlock (const std::string& inName, const std::string& inType) {
    
    if (inType == "source") {
        return std::make_unique <SIM::COM::Source> (inName);
    }
    
    return std::unique_ptr <SIM::Block> ();
};

