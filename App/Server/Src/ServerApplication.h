
#ifndef _SERVERAPPLICATION_H_
#define _SERVERAPPLICATION_H_

#include "Application.h"

/**
 * The Server Application is a command line interface to the 
 * HTTP server. It serves as the heart of this software suite.
 */
class ServerApplication : public APP::Application {

public:
    /**
     * Constructor
     */
    ServerApplication ();
    
    /**
     * Destructor
     */
    virtual ~ServerApplication ();

    /**
     * Run the application
     */
    int Run () override;

};

#endif // _SERVERAPPLICATION_H_