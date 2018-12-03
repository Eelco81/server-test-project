
#ifndef _SIMENDPOINT_H_
#define _SIMENDPOINT_H_

#include <memory>
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpEndpoint.h"
#include "SimService.h"

namespace API { namespace SIM {

class EndPoint : public HTTP::RegexEndPoint {

public:
    /**
     * Constructor
     */
    EndPoint (const std::string& inPath, std::shared_ptr<::SIM::IService> inService);
    
    /**
     * Virtual Destructor
     */
    virtual ~EndPoint ();

protected:
    std::shared_ptr<::SIM::IService> mService;
};

} } // end namespave API::SIM

#endif // _SIMENDPOINT_H_
