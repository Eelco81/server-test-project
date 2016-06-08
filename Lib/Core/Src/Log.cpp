
#include "Log.h"

#pragma warning(disable:4996)

#include <iostream>
#include <chrono>
#include <ctime>
#include <algorithm>

OS::Log::Log () :
    mLevel (kInfo),
    kLevelMap ({ { kFatal, "FATAL" }, { kError, "ERROR" }, { kWarning, "WARN " }, { kInfo, "INFO " }, { kDebug, "DEBUG" }, { kTrace, "TRACE" } })
{
}

OS::Log::~Log () {
}

void OS::Log::Initialize (const std::string& inLevel) {
    auto it = std::find_if (kLevelMap.begin (), kLevelMap.end (), [&inLevel] (const std::pair <Levels, std::string>& inPair) {
        return inPair.second == inLevel;
    });
    if (it != kLevelMap.end ()) {
        mLevel = it->first;
    }
}

void OS::Log::Initialize (OS::Log::Levels inLevel) {
    mLevel = inLevel;
}

void OS::Log::LogMessage (OS::Log::Levels inLevel, const std::string& inMessage) {

    if (inLevel <= mLevel) {

        std::time_t now = std::time (NULL);
        std::tm * ptm = std::localtime (&now);
        char buffer [32];
        std::strftime (buffer, 32, "%d.%m.%Y %H:%M:%S", ptm);

        std::string message = std::string (buffer) + std::string (" - ") + kLevelMap.at (inLevel) + std::string (" - ") + inMessage;

        mMutex.lock ();
        mMessages.push (message);
        mMutex.unlock ();
    }
}

void OS::Log::Flush () {
    
    mMutex.lock ();
    while (mMessages.size () > 0) {
        std::cout << mMessages.front () << std::endl;
        mMessages.pop ();
    }
    mMutex.unlock ();
}
