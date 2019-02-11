
#ifndef _OS_VERSION_H_
#define _OS_VERSION_H_

#include <string>

namespace OS {

class Version {
    
public:
    /**
     * Retrieve the application name
     */
    static const std::string& GetApplicationName () {
        return mApplication;
    }
    
    /**
     * Set the application name
     */
    static void SetApplicationName (const std::string& inName) {
        mApplication = inName;
    }
    
    /**
     * Retrieve the application version
     */
    static std::string GetApplicationVersion () {
        return mVersion;
    }
    
private:
    static std::string mApplication;
    static const std::string mVersion;
    
}; // end class Version

} // end namespace OS

#endif // _OS_VERSION_H_
