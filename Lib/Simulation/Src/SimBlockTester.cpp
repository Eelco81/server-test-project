
#include "gtest/gtest.h"

#include "MathMatrix.h"
#include "MathVector.h"
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

class MatrixBlock : public SIM::Block {
public: 
    MatrixBlock () : 
        Block ("Test")
    {
        AddInPort (mInput, "input");
        AddOutPort (mOutput, "output");
        AddParPort (mParameter, "parameter");
    }
public:
    MATH::Matrix<3,1,uint8_t> mInput = 0x01;
    MATH::Vector<3,uint8_t> mOutput = 0x02;
    MATH::Matrix<1,3,uint8_t> mParameter = 0x03;
};

} // end anonymous namespace

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

TEST (SimBlockTester, MatrixPorts) {
    const auto paths = MatrixBlock ().GetAllPorts ();
    ASSERT_EQ (paths.size(), 9u);
    ASSERT_EQ (std::string ("Test.in.input[0][0]"), paths[0].ToString ());
    ASSERT_EQ (std::string ("Test.in.input[1][0]"), paths[1].ToString ());
    ASSERT_EQ (std::string ("Test.in.input[2][0]"), paths[2].ToString ());
    ASSERT_EQ (std::string ("Test.out.output[0][0]"), paths[3].ToString ());
    ASSERT_EQ (std::string ("Test.out.output[1][0]"), paths[4].ToString ());
    ASSERT_EQ (std::string ("Test.out.output[2][0]"), paths[5].ToString ());
    ASSERT_EQ (std::string ("Test.par.parameter[0][0]"), paths[6].ToString ());
    ASSERT_EQ (std::string ("Test.par.parameter[0][1]"), paths[7].ToString ());
    ASSERT_EQ (std::string ("Test.par.parameter[0][2]"), paths[8].ToString ());
    
}