
#ifndef _SIMCONFIGENDPOINT_H_
#define _SIMCONFIGENDPOINT_H_

#include "SimEndPoint.h"

namespace API { namespace SIM {

class ConfigEndPoint : public EndPoint {

public:
    /**
     * Constructor
     */
    ConfigEndPoint (const std::string& inPath, std::shared_ptr<::SIM::IService> inService, const std::string& inConfigPath);
    
    /**
     * Virtual destructor
     */
    virtual ~ConfigEndPoint ();
    
    /**
     * Get simulation config
     */
    virtual void Get (const HTTP::Request& inRequest, HTTP::Response& outResponse) override;
    
private:
    std::string mConfigPath;
  
};

} } // end namespace API::SIM


#endif // _SIMCONFIGENDPOINT_H_ 