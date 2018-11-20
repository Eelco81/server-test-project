
#ifndef _SIMULATIONENDPOINTS_H_
#define _SIMULATIONENDPOINTS_H_

#include <memory>
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpEndpoint.h"

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

class SimGetValuesEndPoint : public SimEndPoint {

public:
    /**
     * Constructor
     */
    SimGetValuesEndPoint (const std::string& inPath, std::shared_ptr<SIM::IService> inService);
    
    /**
     * Virtual destructor
     */
    virtual ~SimGetValuesEndPoint ();

    /**
     * Overloaded endpoint exectution
     */
    virtual void Execute (const HTTP::Request& inRequest, HTTP::Response& outResponse) override;
};

class SimGetValueEndPoint : public SimEndPoint {

public:
    /**
     * Constructor
     */
    SimGetValueEndPoint (const std::string& inPath, std::shared_ptr<SIM::IService> inService);
    
    /**
     * Virtual destructor
     */
    virtual ~SimGetValueEndPoint ();

    /**
     * Overloaded endpoint exectution
     */
    virtual void Execute (const HTTP::Request& inRequest, HTTP::Response& outResponse) override;
};

class SimSetValueEndPoint : public SimEndPoint {

public:
    /**
     * Constructor
     */
    SimSetValueEndPoint (const std::string& inPath, std::shared_ptr<SIM::IService> inService);
    
    /**
     * Virtual destructor
     */
    virtual ~SimSetValueEndPoint ();

    /**
     * Overloaded endpoint exectution
     */
    virtual void Execute (const HTTP::Request& inRequest, HTTP::Response& outResponse) override;
};

}

#endif // _SIMULATIONENDPOINTS_H_
