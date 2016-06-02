
#ifndef _THREAD_H_
#define _THREAD_H_

#include <memory>
#include <thread>
#include <string>

namespace Core {

class Thread {

public:
    enum Status {
        kUnstarted,
        kRunning,
        kDone
    };

public:
    Thread () = delete;
    Thread (const std::string& inName);
    Thread (const Thread& inThread) = delete;
    virtual ~Thread ();

public:
    inline const std::string& GetName () const { return mName; }
    inline Status GetStatus () const  { return mStatus; }

public:
    void Spawn ();
    void Join ();

public:
    virtual void Execute () = 0;

public:
    // The execute function must be public, it will be called by a C-style function
    // which otherwise would not be able to access it
    virtual void Run ();

private:
    std::string mName;
    Status mStatus;
    std::unique_ptr <std::thread> mImplementation;
};

} // end namespace

#endif
