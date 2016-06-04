
#include "gmock/gmock.h"

#include "Task.h"
#include "PeriodicThread.h"

namespace {

    class TestTask : public App::Task {
    public:
        TestTask (const std::string& inName) : Task (inName) {}
        ~TestTask () {}
        MOCK_METHOD0 (Step, bool ());
    };

}

TEST (PeriodicThreadTester, GetName) {

    App::PeriodicThread thread ("PeriodicThread", 0u);
    ASSERT_EQ ("PeriodicThread", thread.GetName ());   
}

TEST (PeriodicThreadTester, TasksInOneThread) {

    App::PeriodicThread thread ("PeriodicThread", 0u);

    std::unique_ptr <TestTask> task1 (new TestTask ("Task1"));
    std::unique_ptr <TestTask> task2 (new TestTask ("Task2"));
    
    EXPECT_CALL (*task1, Step ())
        .WillOnce (::testing::Return (true))
        .WillOnce (::testing::Return (true))
        .WillOnce (::testing::Return (false));

    EXPECT_CALL (*task2, Step ())
        .WillOnce (::testing::Return (true))
        .WillOnce (::testing::Return (true));

    thread.AddTask (std::move (task1));
    thread.AddTask (std::move (task2));

    thread.Spawn ();
    thread.Join ();

}

TEST (PeriodicThreadTester, TasksInMultipleThreads) {

    App::PeriodicThread thread1 ("PeriodicThread1", 0u);
    App::PeriodicThread thread2 ("PeriodicThread1", 0u);

    std::unique_ptr <TestTask> task1 (new TestTask ("Task1"));
    std::unique_ptr <TestTask> task2 (new TestTask ("Task2"));

    EXPECT_CALL (*task1, Step ())
        .WillOnce (::testing::Return (true))
        .WillOnce (::testing::Return (true))
        .WillOnce (::testing::Return (false));

    EXPECT_CALL (*task2, Step ())
        .WillOnce (::testing::Return (true))
        .WillOnce (::testing::Return (true))
        .WillOnce (::testing::Return (false));

    thread1.AddTask (std::move (task1));
    thread2.AddTask (std::move (task2));

    thread1.Spawn ();
    thread2.Spawn ();

    thread1.Join ();
    thread2.Join ();

}