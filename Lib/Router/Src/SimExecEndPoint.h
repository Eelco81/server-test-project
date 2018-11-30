
#ifndef _SIMEXECENDPOINT_H_
#define _SIMEXECENDPOINT_H_

#include "SimEndPoint.h"

namespace API { namespace SIM {

class ExecEndPoint : public EndPoint {

public:
    /**
     * Constructor
     */
    ExecEndPoint (const std::string& inPath, std::shared_ptr<::SIM::IService> inService, const std::string& inConfigPath);
    
    /**
     * Virtual destructor
     */
    virtual ~ExecEndPoint ();
    
    /**
     * Get simulation status
     */
    virtual void Get (const HTTP::Request& inRequest, HTTP::Response& outResponse) override;

    /**
     * Start simulation
     */
    virtual void Put (const HTTP::Request& inRequest, HTTP::Response& outResponse) override;

    /**
     * Stop simulation
     */
    virtual void Delete (const HTTP::Request& inRequest, HTTP::Response& outResponse) override;
    
private:
    std::string mConfigPath;
};

} } // end namespace API::SIM

#endif // _SIMEXECENDPOINT_H_
