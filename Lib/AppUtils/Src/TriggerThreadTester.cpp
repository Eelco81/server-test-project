
#include "gmock/gmock.h"
#include "TriggerThread.h"
#include "Timing.h"

namespace {

class TestObject  {
public:
    bool Trigger () {
        mTicks.push_back (OS::Timing::Now ());
        return true;
    };
    std::vector<uint64_t> mTicks;
};

}

class TriggerThreadTester : public ::testing::TestWithParam<uint64_t> {};

INSTANTIATE_TEST_CASE_P (TriggerThreadTester, TriggerThreadTester,
    ::testing::Values ( 5000u, 10000u, 20000u, 50000u, 100000u )
);

TEST_P (TriggerThreadTester, Trigger) {
    
    const uint64_t duration (GetParam ());
    
    TestObject object;
    APP::TriggerThread <TestObject> thread ("TriggerThread", duration, &object, &TestObject::Trigger);
    
    thread.Spawn ();
    
    OS::Timing::Sleep (200u + 14u * duration);
    
    thread.Kill ();
    
    OS::Timing::Sleep (duration);
    
    ASSERT_GE (object.mTicks.size (), 9u);
    for (std::size_t i (3u); i < object.mTicks.size (); i++) {
        ASSERT_NEAR (object.mTicks[i] - object.mTicks[i-1], duration, 1000u);
    }
    
}
