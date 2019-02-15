
#ifndef _CLIENTAPPLICATION_H_
#define _CLIENTAPPLICATION_H_

#include "Application.h"

/**
 * The Client Application is a command line interface to the 
 * HTTP remote client. It functions as a 'curl' like tool. 
 * It is mainly used for development and integration testing.
 */
class ClientApplication : public APP::Application {

public:
    /**
     * Constructor
     */
    ClientApplication ();
    
    /**
     * Destructor
     */
    virtual ~ClientApplication ();

    /**
     * Run the application
     */
    int Run () override;

};

#endif // _CLIENTAPPLICATION_H_