
#include "gmock/gmock.h"

#include "Task.h"
#include "PeriodicThread.h"

namespace {

    class TestTask : public APP::Task {
    public:
        TestTask (const std::string& inName) : Task (inName) {}
        MOCK_METHOD0 (Step, bool ());
    };

}

TEST (PeriodicThreadTester, GetName) {

    APP::PeriodicThread thread ("PeriodicThread", 0u);
    ASSERT_EQ ("PeriodicThread", thread.GetName ());   
}

TEST (PeriodicThreadTester, TasksInOneThread) {

    APP::PeriodicThread thread ("PeriodicThread", 0u);

    auto task1 = std::make_unique <TestTask> ("Task1");
    auto task2 = std::make_unique <TestTask> ("Task2");
    
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

    APP::PeriodicThread thread1 ("PeriodicThread1", 1u);
    APP::PeriodicThread thread2 ("PeriodicThread1", 1u);

    auto task1 = std::make_unique <TestTask> ("Task1");
    auto task2 = std::make_unique <TestTask> ("Task2");

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
