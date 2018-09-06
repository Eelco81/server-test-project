
#include "gtest/gtest.h"

#include "SimBlock.h"
#include "SimConnector.h"

namespace {

class TestBlock : public SIM::Block {
public: 
    TestBlock () : 
        Block ("Test")
    {
        AddInPort (&mInput, "input");
        AddOutPort (&mOutput, "output");
    }
public:
    bool mInput = true;
    bool mOutput = false;
};

}

TEST (SimBlockTester, DataTransfer) {

    TestBlock block1;
    TestBlock block2;
    
    SIM::Connector connector (
        block1.GetOutPort ("output"), 
        block2.GetInPort ("input")
    ); 
    
    ASSERT_TRUE (block1.mInput);
    ASSERT_TRUE (block2.mInput);
    ASSERT_FALSE (block1.mOutput);
    ASSERT_FALSE (block2.mOutput);
    
    connector.Transfer ();
    
    ASSERT_TRUE (block1.mInput);
    ASSERT_FALSE (block2.mInput);
    ASSERT_FALSE (block1.mOutput);
    ASSERT_FALSE (block2.mOutput);
}

TEST (SimBlockTester, NonExistingInputPorts) {
    TestBlock block;
    ASSERT_EQ (nullptr, block.GetInPort ("NotExisting").lock ());
}

TEST (SimBlockTester, NonExistingOutputPorts) {
    TestBlock block;
    ASSERT_EQ (nullptr, block.GetOutPort ("NotExisting").lock ());
}