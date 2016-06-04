
#include "Log.h"

#pragma warning(disable:4996)

#include <iostream>
#include <chrono>
#include <ctime>

OS::Log::Log () :
    mLevel (kInfo)
{
}

OS::Log::~Log () {
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

        std::string message = std::string (buffer) + std::string (" - ") + LevelToString (inLevel) + std::string (" - ") + inMessage;

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

std::string OS::Log::LevelToString (OS::Log::Levels inLevel) const {
    switch (inLevel) {
    case kFatal:
        return "FATAL";
    case kError: 
        return "ERROR";
    case kWarning:
        return "WARN ";
    case kInfo:
        return "INFO ";
    case kDebug:
        return "DEBUG";
    case kTrace:
        return "TRACE";
    default:
        return "?????";
    }
}