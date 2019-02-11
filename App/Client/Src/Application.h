
#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "CommandLine.h"

/**
 * The Client Application is a command line interface to the 
 * HTTP remote client. It functions as a 'curl' like tool. 
 * It is mainly used for development and integration testing.
 */
namespace Application {

/**
 * Run the application
 */
void Run (const OS::CommandLine& inCommandLine);

};

#endif // _APPLICATION_H_