
#ifdef _WIN32
#pragma warning(disable:4996)
#endif

#include <iostream>
#include <algorithm>

#include "Log.h"
#include "Timing.h"

OS::Log::Log () :
    mLevel (kInfo),
    kLevelMap ({ {kNone, "NONE" }, { kFatal, "FATAL" }, { kError, "ERROR" }, { kWarning, "WARN " }, { kInfo, "INFO " }, { kDebug, "DEBUG" }, { kTrace, "TRACE" } })
{
}

OS::Log::~Log () {
    Flush ();
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

    if (mLevel == Levels::kNone) {
        return;
    }
    
    if (inLevel <= mLevel) {
        const auto date (OS::Timing::ToString (OS::Timing::Now()));
        const std::string message (date + std::string (" - ") + kLevelMap.at (inLevel) + std::string (" - ") + inMessage);

        SingleLock lock (mMutex);
        mMessages.push (message);
    }
}

void OS::Log::Flush () {
    
    SingleLock lock (mMutex);
    while (mMessages.size () > 0) {
        std::cout << mMessages.front () << std::endl;
        mMessages.pop ();
    }
}

OS::LogBuffer::LogBuffer (Log::Levels inLevel) :
    mLevel (inLevel)
{
}

OS::LogBuffer::~LogBuffer () {
    Log::Instance ().LogMessage (mLevel, mStream.str ());
}
