
#include "Log.h"

#pragma warning(disable:4996)

#include <iostream>
#include <chrono>
#include <ctime>

Core::Log::Log () :
    mLevel (kInfo)
{
}

Core::Log::~Log () {
}

void Core::Log::Initialize (Core::Log::Levels inLevel) {
    mLevel = inLevel;
}

void Core::Log::LogMessage (Core::Log::Levels inLevel, const std::string& inMessage) {

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

void Core::Log::Flush () {
    
    mMutex.lock ();
    while (mMessages.size () > 0) {
        std::cout << mMessages.front () << std::endl;
        mMessages.pop ();
    }
    mMutex.unlock ();
}

std::string Core::Log::LevelToString (Core::Log::Levels inLevel) const {
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