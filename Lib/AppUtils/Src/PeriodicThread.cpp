
#include "PeriodicThread.h"
#include "Task.h"

#include <chrono>

APP::PeriodicThread::PeriodicThread (const std::string& inName, unsigned inWaitTime) :
    Thread (inName),
    mWaitTime (inWaitTime),
    mRunning (false)
{
}

APP::PeriodicThread::~PeriodicThread () {
}

bool APP::PeriodicThread::IsRunning () {
    return mRunning;
}

void APP::PeriodicThread::AddTask (std::unique_ptr<Task> inTask) {
    mTasks.push_back (std::move (inTask));
}

void APP::PeriodicThread::Execute () {
    
    mRunning = true;

    while (mRunning) {

        for (auto& it : mTasks) {
            if (!it->Step ()) {
                mRunning = false;
                break;
            }
        }
        std::this_thread::sleep_for (std::chrono::milliseconds (mWaitTime));
    }

}