
#include "gmock/gmock.h"
#include <string>

#include "Thread.h"

namespace {

    class TestThread : public Core::Thread {
    public:
        TestThread () : Thread ("TestThread") {}
        ~TestThread () {}

        MOCK_METHOD0 (Execute, void ());
    };

}

TEST (ThreadTester, ThreadName) {

    TestThread thread;

    ASSERT_EQ ("TestThread", thread.GetName ());
}

TEST (ThreadTester, SpawningAndJoiningThreads) {

    TestThread thread;

    EXPECT_CALL (thread, Execute ()).Times (1);

    thread.Spawn ();
    thread.Join ();
}

TEST (ThreadTester, JoiningNonSpawnedThreads) {

    TestThread thread;

    EXPECT_CALL (thread, Execute ()).Times (0);
    thread.Join ();
}