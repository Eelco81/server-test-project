
#ifndef _APP_APPLICATION_H_
#define _APP_APPLICATION_H_

#include <string>
#include "SupportThread.h"
#include "CommandLine.h"

namespace APP {

/**
 * The abstract class Application form the base of 
 * command line applications.
 */
class Application {

public:
    /**
     * Constructor
     */
    Application (const std::string& inAppName);
    
    /**
     * Destructor
     */
    virtual ~Application ();

    /**
     * Execute the application.
     */
    int Execute (int argc, char** argv);
    
    /**
     * Main function to be overloaded
     */
    virtual int Run () = 0;
    
protected:
    SupportThread mSupportThread;
    OS::CommandLine mCommandLine;
};

}

#endif // _APP_APPLICATION_H_
