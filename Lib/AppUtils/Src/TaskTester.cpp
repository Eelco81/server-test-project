
#include "gtest/gtest.h"
#include <string>

#include "Task.h"

namespace {

    class TestTask : public Core::Task {
    public:
        TestTask () : Task ("TestTask") {}
        ~TestTask () {}
        virtual bool Step () override {
            return true;
        }
    };

}

TEST (TaskTester, TaskName) {
    TestTask task;
    ASSERT_EQ ("TestTask", task.GetName ());
}