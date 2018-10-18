
#ifndef _SIMULATIONENDPOINTS_H_
#define _SIMULATIONENDPOINTS_H_

#include <memory>
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpEndPoint.h"

namespace SIM {
    class IService;
}

namespace API {

class SimEndPoint : public HTTP::EndPoint {

public:
    /**
     * Constructor
     */
    SimEndPoint (const std::string& inPath, HTTP::Method inMethod, std::shared_ptr<SIM::IService> inService);
    
    /**
     * Virtual Destructor
     */
    virtual ~SimEndPoint ();

protected:
    std::shared_ptr<SIM::IService> mService;
};

class SimLoadEndPoint : public SimEndPoint {

public:
    /**
     * Constructor
     */
    SimLoadEndPoint (const std::string& inPath, std::shared_ptr<SIM::IService> inService);
    
    /**
     * Virtual destructor
     */
    virtual ~SimLoadEndPoint ();
    
    /**
     * Overloaded endpoint exectution
     */
    virtual void Execute (const HTTP::Request& inRequest, HTTP::Response& outResponse) override;
};

class SimStartEndPoint : public SimEndPoint {

public:
    /**
     * Constructor
     */
    SimStartEndPoint (const std::string& inPath, std::shared_ptr<SIM::IService> inService);
    
    /**
     * Virtual destructor
     */
    virtual ~SimStartEndPoint ();

    /**
     * Overloaded endpoint exectution
     */
    virtual void Execute (const HTTP::Request& inRequest, HTTP::Response& outResponse) override;
};

class SimStopEndPoint : public SimEndPoint {

public:
    /**
     * Constructor
     */
    SimStopEndPoint (const std::string& inPath, std::shared_ptr<SIM::IService> inService);
    
    /**
     * Virtual destructor
     */
    virtual ~SimStopEndPoint ();

    /**
     * Overloaded endpoint exectution
     */
    virtual void Execute (const HTTP::Request& inRequest, HTTP::Response& outResponse) override;
};

class SimGetPathsEndPoint : public SimEndPoint {

public:
    /**
     * Constructor
     */
    SimGetPathsEndPoint (const std::string& inPath, std::shared_ptr<SIM::IService> inService);
    
    /**
     * Virtual destructor
     */
    virtual ~SimGetPathsEndPoint ();

    /**
     * Overloaded endpoint exectution
     */
    virtual void Execute (const HTTP::Request& inRequest, HTTP::Response& outResponse) override;
};


}

#endif // _SIMULATIONENDPOINTS_H_
