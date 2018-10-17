
#ifndef _SIMCOM_FACTORY_
#define _SIMCOM_FACTORY_

#include "SimFactory.h"

namespace SIM { namespace COM {

class Factory : public SIM::Factory {
    std::unique_ptr<SIM::Block> CreateBlock (const std::string& inName, const std::string& inType);
};

}} // end namespace SIM::COM

#endif // _SIMCOM_FACTORY_
