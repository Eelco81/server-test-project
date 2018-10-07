
#ifndef _LOG_H_
#define _LOG_H_

#include <string>
#include <mutex>
#include <queue>
#include <map>
#include <sstream>

#include "Macros.h"
#include "Mutex.h"

/**
 * Convenience definitions
 */
#define LOGMESSAGE OS::Log::Instance ().LogMessage

#define LOGFATAL OS::LogBuffer (OS::Log::Levels::kFatal)
#define LOGERROR OS::LogBuffer (OS::Log::Levels::kError)
#define LOGWARNING OS::LogBuffer (OS::Log::Levels::kWarning)
#define LOGINFO OS::LogBuffer (OS::Log::Levels::kInfo)
#define LOGDEBUG OS::LogBuffer (OS::Log::Levels::kDebug)
#define LOGTRACE OS::LogBuffer (OS::Log::Levels::kTrace)

namespace OS {

class Log {

    NO_COPY_CONSTRUCTORS (Log);

public:

    /**
     * Supported log levels, in order of verbosity.
     */
    enum Levels {
        kNone,
        kFatal,
        kError,
        kWarning,
        kInfo,
        kDebug,
        kTrace
    };
    
    /**
     * Get the instance of the logger (singleton)
     */
    static Log& Instance (){
        static Log instance;
        return instance;
    }

public:
    /**
     * Initialize the logger 
     */
    void Initialize (Levels inLevel);
    
    /**
     * Initialize the logger 
     */
    void Initialize (const std::string& inLevelString);
    
    /**
     * Log a message to the logger
     */
    void LogMessage (Levels inLevel, const std::string& inMessage);
    
    /**
     * Flush the logger. 
     */
    void Flush ();

private:
    /**
     * Private constructor (must use singleton instance).
     */
    Log ();
    
    /**
     * Destructor
     */
    ~Log ();

private:
    Mutex mMutex;
    std::queue <std::string> mMessages;
    Levels mLevel;
    const std::map <Levels, std::string> kLevelMap;

};

class LogBuffer {

public:
    LogBuffer (Log::Levels inLevel) :
        mLevel (inLevel)
    {
    }
    ~LogBuffer () {
        Log::Instance ().LogMessage (mLevel, mStream.str ());
    }
    
    template <typename T>
    LogBuffer& operator<< (const T& inValue) {
        mStream << inValue;
        return *this;
    }
    
private:
    std::stringstream mStream;
    Log::Levels mLevel;
};



}


#endif
