
#ifndef _OS_MUTEX_H_
#define _OS_MUTEX_H_

#include <mutex>

namespace OS {

class Mutex {
public:
    void Lock ();
    void UnLock ();
private:
    std::mutex mMutex;
};

class SingleLock {
public:
    SingleLock (Mutex& inMutex);
    ~SingleLock ();
private:
    Mutex& mMutex;
};

}

#endif // _OS_MUTEX_H_
