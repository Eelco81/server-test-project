
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
        AddParPort (&mParameter, "parameter");
    }
public:
    bool mInput = true;
    bool mOutput = false;
    bool mParameter = false;
};

}

TEST (SimBlockTester, DataTransfer) {

    TestBlock block1;
    TestBlock block2;
    
    SIM::Connector connector (
        block1.GetPort (SIM::Path ("Test", "output", SIM::Path::OUTPUT)), 
        block2.GetPort (SIM::Path ("Test", "input", SIM::Path::INPUT))
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
    ASSERT_EQ (nullptr, block.GetPort (SIM::Path ("Test", "NotExisting", SIM::Path::INPUT)).lock ());
}

TEST (SimBlockTester, NonExistingOutputPorts) {
    TestBlock block;
    ASSERT_EQ (nullptr, block.GetPort (SIM::Path ("Test", "NotExisting", SIM::Path::OUTPUT)).lock ());
}

TEST (SimBlockTester, NonExistingParameterPorts) {
    TestBlock block;
    ASSERT_EQ (nullptr, block.GetPort (SIM::Path ("Test", "NotExisting", SIM::Path::PARAMETER)).lock ());
}

TEST (SimBlockTester, GetAllPorts) {
    const auto paths = TestBlock ().GetAllPorts ();
    ASSERT_EQ (paths.size(), 3u);
    ASSERT_EQ (std::string ("Test.in.input"), paths[0].ToString ());
    ASSERT_EQ (std::string ("Test.out.output"), paths[1].ToString ());
    ASSERT_EQ (std::string ("Test.par.parameter"), paths[2].ToString ());
}