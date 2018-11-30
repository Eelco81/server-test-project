
#ifndef _SIMPORTSENDPOINTS_H_
#define _SIMPORTSENDPOINTS_H_

#include "SimEndPoint.h"

namespace API { namespace SIM {

class PortsEndPoint : public EndPoint {

public:
    /**
     * Constructor
     */
    PortsEndPoint (const std::string& inPath, std::shared_ptr<::SIM::IService> inService);
    
    /**
     * Virtual destructor
     */
    virtual ~PortsEndPoint ();

    /**
     * Get all port values
     */
    virtual void Get (const HTTP::Request& inRequest, HTTP::Response& outResponse) override;
};

} } // end namespace API::SIM

#endif // _SIMPORTSENDPOINTS_H_
