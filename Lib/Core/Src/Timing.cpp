
#include "Timing.h"
#include "Log.h"

#include <thread>
#include <chrono>
#include <ctime>

#if (defined _WIN32 || defined _WIN64 || defined __CYGWIN__)

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

extern "C" NTSYSAPI NTSTATUS NTAPI NtSetTimerResolution(ULONG DesiredResolution, BOOLEAN SetResolution, PULONG CurrentResolution);

#endif

void OS::Timing::Initialize () {

#if (defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
    ULONG currentRes;
    NtSetTimerResolution (1, TRUE, &currentRes);
#endif
}

uint64_t OS::Timing::Now () {

    const auto now (std::chrono::high_resolution_clock::now ());
    const auto duration (now.time_since_epoch ());
    
    return std::chrono::duration_cast<std::chrono::microseconds> (duration).count ();
}

std::string OS::Timing::ToString (uint64_t inTime) {
    
    std::time_t now (inTime / 1000u / 1000u);
    std::tm * ptm = std::localtime (&now);
    char buffer [32];
    std::strftime (buffer, 32, "%d.%m.%Y %H:%M:%S", ptm);
    
    return std::string (buffer);
}

std::string OS::Timing::ToStdString (uint64_t inTime) {
    
    std::time_t now (inTime / 1000u / 1000u);
    std::tm * ptm = std::localtime (&now);
    char buffer [32];
    std::strftime (buffer, 32, "%a, %d %b %Y %H:%M:%S %Z", ptm);
    
    return std::string (buffer);
}

void OS::Timing::Sleep (uint64_t inSleep) {
    
    int milliseconds = inSleep / 1000;
    int ms_remaining = (milliseconds) % 1000;
    long usec = ms_remaining * 1000;
    
    struct timespec ts_sleep;
    ts_sleep.tv_sec = (milliseconds) / 1000;
    ts_sleep.tv_nsec = 1000*usec;
    
    nanosleep (&ts_sleep, NULL);
}
