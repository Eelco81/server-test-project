
#ifndef _SIMPORTENDPOINT_H_
#define _SIMPORTENDPOINT_H_

#include "SimEndPoint.h"

namespace API { namespace SIM {

class PortEndPoint : public EndPoint {

public:
    /**
     * Constructor
     */
    PortEndPoint (const std::string& inPath, std::shared_ptr<::SIM::IService> inService);
    
    /**
     * Virtual destructor
     */
    virtual ~PortEndPoint ();

    /**
     * Get a value
     */
    virtual void Get (const HTTP::Request& inRequest, HTTP::Response& outResponse) override;
    
    /**
     * Set a value
     */
    virtual void Put (const HTTP::Request& inRequest, HTTP::Response& outResponse) override;
};

} } // end namespace API::SIM

#endif // _SIMPORTENDPOINT_H_
