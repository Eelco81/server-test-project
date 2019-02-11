
#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "CommandLine.h"

/**
 * The Server Application is a command line interface to the 
 * HTTP server. It serves as the heart of this software suite.
 */
namespace Application {

/**
 * Run the application
 */
void Run (const OS::CommandLine& inCommandLine);

};

#endif // _APPLICATION_H_