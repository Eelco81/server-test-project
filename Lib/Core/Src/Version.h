
#ifndef _OS_VERSION_H_
#define _OS_VERSION_H_

#include <string>

namespace OS {

namespace Version {
    
    /**
     * Retrieve the application name
     */
    static std::string GetApplicationName () {
        return std::string ("HttpServer");
    }
    
    /**
     * Retrieve the application version
     */
    static std::string GetApplicationVersion () {
        return std::string ("0.0.1");
    }
    
} // end namespace Version

} // end namespace OS

#endif // _OS_VERSION_H_
