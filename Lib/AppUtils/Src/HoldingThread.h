
#ifndef _APP_HOLDTHREAD_H_
#define _APP_HOLDTHREAD_H_

#include "Thread.h"

namespace APP {

template <typename T>
class HoldingThread : public OS::Thread {
    
public:
    /**
     * Constructor
     */
    HoldingThread (const std::string& inName, T* inObject, void(T::*inHoldFunction)()) :
        Thread (inName),
        mObject (inObject),
        mHoldFunction (inHoldFunction)
    {
    }
    
    /**
     * Destructor
     */
    virtual ~HoldingThread () {
        // User must kill the thread execution before destructor is called
        Join ();
    }
     
    /**
     * Thread execution function
     */
    void Execute () override {
        (mObject->*mHoldFunction)();
    }
    
    /**
     * Empty kill function. This must be implemented in hold function callback
     */
    void Kill () override {
    }
    
private:
    T* mObject;
    void(T::*mHoldFunction)();
};

} // end namespace APP



#endif // _APP_HOLDTHREAD_H_
