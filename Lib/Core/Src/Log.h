
#ifndef _LOG_H_
#define _LOG_H_

#include <string>
#include <mutex>
#include <queue>
#include <map>

#include "Macros.h"
#include "Mutex.h"

#define LOGMESSAGE OS::Log::Instance ().LogMessage

namespace OS {

class Log {

    NO_COPY_CONSTRUCTORS (Log);

public:
    enum Levels {
        kFatal,
        kError,
        kWarning,
        kInfo,
        kDebug,
        kTrace
    };

public: 
    static Log& Instance (){
        static Log instance;
        return instance;
    }

protected:
    Log ();
    ~Log ();

public:
    void Initialize (Levels inType);
    void Initialize (const std::string& inTypeString);
    void LogMessage (Levels inType, const std::string& inMessage);
    void Flush ();

private:
    Mutex mMutex;
    std::queue <std::string> mMessages;
    
    Levels mLevel;
    const std::map <Levels, std::string> kLevelMap;

};

}

#endif
