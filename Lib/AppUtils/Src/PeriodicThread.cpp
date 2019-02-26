
#include "Timing.h"
#include "PeriodicThread.h"
#include "Task.h"
#include "Log.h"

APP::PeriodicThread::PeriodicThread (const std::string& inName, uint64_t inWaitTime) :
    Thread (inName),
    mWaitTime (inWaitTime),
    mRunning (false)
{
}

APP::PeriodicThread::~PeriodicThread () {
    Kill ();
    Join ();
}

void APP::PeriodicThread::AddTask (std::unique_ptr<Task> inTask) {
    mTasks.push_back (std::move (inTask));
}

void APP::PeriodicThread::Execute () {
    
    mRunning = true;

    while (mRunning) {
        
        const auto tick (OS::Timing::Now ());
        for (const auto& it : mTasks) {
            if (!it->Step ()) {
                mRunning = false;
                break;
            }
        }
        
        const auto duration (OS::Timing::Now () - tick);
        const auto sleeptime (mWaitTime - duration);
        
        // LOGINFO << GetName() << " " << duration << " " << mWaitTime << " " << sleeptime;
        
        if (mWaitTime > 0u && duration >= mWaitTime) {
            LOGWARNING << "Overrun in " << GetName() << ": WaitTime [" << mWaitTime << "ms] Duration [" << duration << "]";
        }
        else if (mWaitTime > 0u) {
            OS::Timing::Sleep (sleeptime);
        }
    }

}

void APP::PeriodicThread::Kill () {
    mRunning = false;
}