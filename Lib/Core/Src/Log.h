
#ifndef _LOG_H_
#define _LOG_H_

#include <string>
#include <mutex>
#include <queue>

namespace OS {

class Log {

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
    Log (Log const&) = delete;
    Log (Log&&) = delete;
    Log& operator=(Log const&) = delete;
    Log& operator=(Log &&) = delete;

    static Log& Instance (){
        static Log instance;
        return instance;
    }

protected:
    Log ();
    ~Log ();

public:
    void Initialize (Levels inType);
    void LogMessage (Levels inType, const std::string& inMessage);
    void Flush ();

private:
    std::string LevelToString (Levels inLevel) const;

private:
    std::mutex mMutex;
    std::queue <std::string> mMessages;

    Levels mLevel;

};

}

#endif
