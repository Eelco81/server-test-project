
#include "gtest/gtest.h"

#include "SimBlock.h"
#include "SimConnector.h"

#include "MathMatrix.h"

namespace {

class TestBlock : public SIM::Block {

public: 
    TestBlock () : 
        Block ("Test"),
        mInput (true),
        mOutput (false)
    {
        AddInPort (&mInput, "input");
        AddOutPort (&mOutput, "output");
    }
    virtual ~TestBlock () {
    }

public:
    bool mInput;
    bool mOutput;

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
