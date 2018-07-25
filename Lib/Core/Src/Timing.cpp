
#include "Timing.h"

#include <thread>
#include <chrono>
#include <ctime>

uint64_t OS::Timing::Now () {
    const auto now (std::chrono::system_clock::now ());
    const auto duration (now.time_since_epoch ());
    return std::chrono::duration_cast<std::chrono::milliseconds> (duration).count ();
}

std::string OS::Timing::ToString (uint64_t inTime) {
    
    std::time_t now (inTime / 1000u);
    std::tm * ptm = std::localtime (&now);
    char buffer [32];
    std::strftime (buffer, 32, "%d.%m.%Y %H:%M:%S", ptm);
    
    return std::string (buffer);
}

void OS::Timing::Sleep (uint64_t inMilliSeconds) {
    std::this_thread::sleep_for (std::chrono::milliseconds (inMilliSeconds));
}