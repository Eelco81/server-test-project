
#include <csignal>
#include <atomic>
#include <condition_variable>
#include "ThreadBlocker.h"

namespace {

std::mutex gMutex;
std::condition_variable gCV;
bool gRaised;

} // end anonymous namespace

void APP::ThreadBlocker (int inSignal) {
    
    gRaised = false;
    
    std::signal (inSignal, [](int sigal) {
        gRaised = true;
        gCV.notify_one ();
    });
    
    std::unique_lock<std::mutex> lock (gMutex);
    gCV.wait (lock, []{return gRaised;});
}
