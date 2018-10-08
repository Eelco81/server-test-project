
#include "gmock/gmock.h"
#include "TriggerThread.h"
#include "Timing.h"

namespace {

class TestObject  {
public:
    MOCK_METHOD0 (Trigger, bool ());
};

}

TEST (TriggerThreadTester, Trigger) {
    
    TestObject object;
    
    EXPECT_CALL (object, Trigger ())
        .WillOnce (::testing::Return (true))
        .WillOnce (::testing::Return (true));
    
    APP::TriggerThread <TestObject> thread ("TriggerThread", 100u, &object, &TestObject::Trigger);
    
    thread.Spawn ();
    OS::Timing::Sleep (150u);
    
    thread.Kill ();
    
    OS::Timing::Sleep (150u);
    
}