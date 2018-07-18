
#ifndef _THREAD_H_
#define _THREAD_H_

#include <memory>
#include <string>
#include <thread>

#include "Macros.h"


namespace OS {

class Thread {

    NO_COPY_CONSTRUCTORS (Thread);

public:
    enum Status {
        kUnstarted,
        kRunning,
        kDone
    };

public:
    Thread () = delete;
    Thread (const std::string& inName);
    virtual ~Thread ();

public:
    inline const std::string& GetName () const { return mName; }
    inline Status GetStatus () const  { return mStatus; }

public:
    void Spawn ();
    void Join ();

public:
    virtual void Execute () = 0;

private:
    void Run ();
	
private:
    std::string mName;
    Status mStatus;
    std::unique_ptr <std::thread> mImplementation;
};

} // end namespace

#endif
