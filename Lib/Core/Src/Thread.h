
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
    /**
     * Status
     */
    enum Status {
        kUnstarted,
        kRunning,
        kDone
    };

    /**
     * Deleted default constructor
     */
    Thread () = delete;
    
    /**
     * Constructor
     */
    Thread (const std::string& inName);
    
    /**
     * Virtual destructor
     */
    virtual ~Thread ();

    /**
     * Get the threads name
     */
    inline const std::string& GetName () const { return mName; }
    
    /**
     * Get the threads running status
     */
    inline Status GetStatus () const  { return mStatus; }

    /**
     * Spawn the thread.
     */
    void Spawn ();
    
    /**
     * Wait for the thread to stop running
     */
    void Join ();
    
    /**
     * Thread execution function
     */
    virtual void Execute () = 0;
    
    /**
     * This function must gracefully stop the execution function
     */
    virtual void Kill() = 0;

private:
    /**
     * Internal thread execution function, wraps Execute () with 
     * some additional bookkeeping and logging
     */
    void Run ();
    
private:
    std::string mName;
    Status mStatus;
    std::unique_ptr <std::thread> mImplementation;
};

} // end namespace

#endif
