
#ifndef _OS_MUTEX_H_
#define _OS_MUTEX_H_

#include <mutex>

namespace OS {

class Mutex {

public:
    /**
     * Lock the resource
     */
    void Lock ();
    
    /**
     * Unlock the resource
     */
    void UnLock ();

private:
    std::mutex mMutex;

};

class SingleLock {

public:
    /** 
     * Constructor (lock the resource)
     */
    SingleLock (Mutex& inMutex);
    
    /**
     * Destructor (unlock the resource)
     */
    ~SingleLock ();

private:
    Mutex& mMutex;

};

}

#endif // _OS_MUTEX_H_
