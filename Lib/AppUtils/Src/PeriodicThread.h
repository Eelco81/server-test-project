
#ifndef _PERIODICTHREAD_H_
#define _PERIODICTHREAD_H_

#include <cstdint>
#include <vector>
#include <memory>

#include "Macros.h"
#include "Thread.h"

namespace APP {

class Task;

class PeriodicThread : public OS::Thread {

    NO_COPY_CONSTRUCTORS (PeriodicThread);

public:
    PeriodicThread () = delete;
    PeriodicThread (const std::string& inName, uint64_t inWaitTime);
    virtual ~PeriodicThread (); 

public:
    bool IsRunning ();
    void AddTask (std::unique_ptr<Task> inTask);
    void Execute () override;
    void Kill() override;
    
private:
    std::vector<std::unique_ptr<Task>> mTasks;
    uint64_t mWaitTime;
    bool mRunning;
};

}

#endif
