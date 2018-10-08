
#ifndef _OS_TRIGGERTHREAD_H_
#define _OS_TRIGGERTHREAD_H_

#include "PeriodicThread.h"
#include "Task.h"

namespace APP {

template <typename T>
class TriggerThread : public PeriodicThread {
    
private:
    class TriggerTask : public Task {
    public: 
        TriggerTask (const std::string& inName, T* inObject, bool(T::*inTriggerFunction)()) :
            Task (inName),
            mObject (inObject),
            mTriggerFunction (inTriggerFunction)
        {
        }
        ~TriggerTask () = default;
        bool Step () override {
            return (mObject->*mTriggerFunction) ();
        }
    private:
        T* mObject;
        bool(T::*mTriggerFunction)();
    };
    
public:
    TriggerThread (const std::string& inName, uint64_t inWaitTime, T* inObject, bool(T::*inTriggerFunction)()) :
        PeriodicThread (inName, inWaitTime)
    {
        AddTask (std::make_unique<TriggerTask> (inName, inObject, inTriggerFunction));
    }
};

} // end namespace APP

#endif // _OS_TRIGGERTHREAD_H_
