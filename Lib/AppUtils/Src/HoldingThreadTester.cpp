
#include "gmock/gmock.h"
#include "HoldingThread.h"


namespace {

class TestObject  {
public:
    MOCK_METHOD0 (Hold, void ());
};

}

TEST (HoldingThreadTester, Hold) {
    
    TestObject object;
    
    EXPECT_CALL (object, Hold ())
        .WillOnce (::testing::Return ());
    
    APP::HoldingThread <TestObject> thread ("HoldingThread", &object, &TestObject::Hold);
    
    thread.Spawn ();
    
}
