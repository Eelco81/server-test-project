
#ifndef _TESTAPPLICATION_H_
#define _TESTAPPLICATION_H_

#include "Application.h"

namespace TEST {

/**
 * The Test Application is designed to facilitate
 * endurance tests for the HTTP, RFC6455 and SSE server
 */
class Application : public APP::Application {

public:
    /**
     * Constructor
     */
    Application ();
    
    /**
     * Destructor
     */
    virtual ~Application ();
    
    /**
     * Run the application
     */
    int Run () override;
    
};

}

#endif // _TESTAPPLICATION_H_