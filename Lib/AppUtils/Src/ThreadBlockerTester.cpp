
#include "gtest/gtest.h"

#include <csignal>
#include "Timing.h"
#include "Thread.h"
#include "ThreadBlocker.h"

namespace {

class TestThread : public OS::Thread {
public:
    TestThread () : Thread ("TestThread") {}
    void Execute () {
        APP::ThreadBlocker (SIGINT);
    }
    void Kill () {
        std::raise (SIGINT);
    }
};

} // end anonymous namespace 


TEST (ThreadBlockerTester, Block) {
    
    TestThread thread;
    
    ASSERT_EQ (OS::Thread::kUnstarted, thread.GetStatus ());
    
    thread.Spawn ();
    OS::Timing::Sleep (50000u);
    
    ASSERT_EQ (OS::Thread::kRunning, thread.GetStatus ());
    
    OS::Timing::Sleep (50000u);
    thread.Kill ();
    thread.Join ();
    ASSERT_EQ (OS::Thread::kDone, thread.GetStatus ());
}
