
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
    /**
     * Constructor, inWaitTime indicates the frequency at which the thread should run
     */
    PeriodicThread (const std::string& inName, uint64_t inWaitTime);
    
    /**
     * Virtual destructor
     */
    virtual ~PeriodicThread (); 

    /**
     * Add a task to the periodic thread execution
     */
    void AddTask (std::unique_ptr<Task> inTask);
    
    /**
     * Thread execution function
     */
    void Execute () override;
    
    /**
     * Stop after the next wait cycle.
     */
    void Kill() override;
    
private:
    std::vector<std::unique_ptr<Task>> mTasks;
    uint64_t mWaitTime;
    bool mRunning;
};

}

#endif
