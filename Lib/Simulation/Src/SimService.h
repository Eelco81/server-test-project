

#ifndef _SIM_SERVICE_H_
#define _SIM_SERVICE_H_

#include "Macros.h"
#include <atomic>

namespace SIM {

class Service {

    NO_COPY_CONSTRUCTORS (Service);
    
public:
    Service ();
    virtual ~Service ();
    
public:
    bool Load ();
    bool UnLoad ();
    bool IsLoaded () const;
    
    bool Start ();
    bool Stop ();
    bool IsRunning () const;

private:
    std::atomic<bool> mIsLoaded;
    std::atomic<bool> mIsRunning;
};

}

#endif // _SIM_SERVICE_H_
