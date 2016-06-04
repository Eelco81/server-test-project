
#ifndef _PERIODICTHREAD_H_
#define _PERIODICTHREAD_H_

#include <vector>
#include <memory>

#include "Thread.h"

namespace APP {

class Task;

class PeriodicThread : public OS::Thread {

public:
    PeriodicThread () = delete;
    PeriodicThread (const std::string& inName, unsigned inWaitTime);
    PeriodicThread (const PeriodicThread& inThread) = delete;
    virtual ~PeriodicThread (); 

public:
    bool IsRunning ();
    void AddTask (std::unique_ptr<Task> inTask);
    void Execute () override;

private:
    std::vector<std::unique_ptr<Task>> mTasks;
    unsigned mWaitTime;
    bool mRunning;
};

}

#endif
