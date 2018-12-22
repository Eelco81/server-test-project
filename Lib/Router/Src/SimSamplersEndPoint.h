

#ifndef _SIMSAMPLERSENDPOINT_H_
#define _SIMSAMPLERSENDPOINT_H_

#include "SimEndPoint.h"

namespace API { namespace SIM {

class SamplersEndPoint : public EndPoint {

public:
    /**
     * Constructor
     */
    SamplersEndPoint (const std::string& inPath, std::shared_ptr<::SIM::IService> inService);
    
    /**
     * Virtual destructor
     */
    virtual ~SamplersEndPoint ();

    /**
     * Get samplers
     */
    virtual void Get (const HTTP::Request& inRequest, HTTP::Response& outResponse) override;

};

} } // end namespace API::SIM

#endif // _SIMSAMPLERSENDPOINT_H_

