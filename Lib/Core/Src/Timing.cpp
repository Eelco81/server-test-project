
#include "Timing.h"

#include <thread>
#include <chrono>


void OS::Timing::Sleep(std::size_t inMilliSeconds) {
    std::this_thread::sleep_for (std::chrono::milliseconds (inMilliSeconds));
}