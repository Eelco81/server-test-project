
#include "Timing.h"
#include "PeriodicThread.h"
#include "Task.h"


APP::PeriodicThread::PeriodicThread (const std::string& inName, uint64_t inWaitTime) :
    Thread (inName),
    mWaitTime (inWaitTime),
    mRunning (false)
{
}

APP::PeriodicThread::~PeriodicThread () = default;

bool APP::PeriodicThread::IsRunning () {
    return mRunning;
}

void APP::PeriodicThread::AddTask (std::unique_ptr<Task> inTask) {
    mTasks.push_back (std::move (inTask));
}

void APP::PeriodicThread::Execute () {
    
    mRunning = true;

    while (mRunning) {

        for (const auto& it : mTasks) {
            if (!it->Step ()) {
                mRunning = false;
                break;
            }
        }
        OS::Timing::Sleep (mWaitTime);
    }

}

void APP::PeriodicThread::Kill() {
    mRunning = false;
}