
#include "gmock/gmock.h"

#include "SimLoop.h"
#include "SimBlock.h"
#include "SimConnector.h"

#include <memory>

namespace {

class TestBlock : public SIM::Block {
public: 
    TestBlock (const std::string& inName) : 
        Block (inName)
    {
        AddInPort<bool> ("input");
        AddOutPort<bool> ("output");
    }
    MOCK_METHOD0 (Configure, void ());
    MOCK_METHOD1 (Initialize, void (double));
    MOCK_METHOD1 (Step, void (double));
    MOCK_METHOD1 (Terminate, void (double));
};

}

class SimLoopNameTester : public ::testing::TestWithParam<std::string> {};

INSTANTIATE_TEST_CASE_P (SimLoopNameTester, SimLoopNameTester,
    ::testing::Values(
        "Not Allowed Name",
        "Not_Allowed_Name",
        "Not,Allowed,Name",
        "Not.Allowed.Name",
        "Not*Allowed*Name",
        "Not^Allowed^Name",
        "Not@Allowed@Name",
        "Not#Allowed#Name",
        "Not!Allowed!Name",
        "Not?Allowed?Name",
        "Not:Allowed:Name"
    )
);

TEST_P (SimLoopNameTester, IllegalNames) {
    
    try {
        SIM::Loop loop (100u);
        loop.AddBlock (std::make_unique<SIM::Block> (GetParam ()));
        ASSERT_FALSE (true);
    }
    catch (std::exception& e) {
        ASSERT_EQ (std::string ("Cannot add block <") + GetParam () + std::string(">, the name does not meet requirements"), e.what ());
    }
}

TEST (SimLoopTester, NonUniqueNames) {
    
    try {
        SIM::Loop loop (100u);
        loop.AddBlock (std::make_unique<SIM::Block> ("Block"));
        loop.AddBlock (std::make_unique<SIM::Block> ("Block"));
        ASSERT_FALSE (true);
    }
    catch (std::exception& e) {
        ASSERT_EQ (std::string ("Cannot add block <Block>, the name is not unique"), e.what ());
    }
}

TEST (SimLoopTester, IllegalPaths) {
    
    try {
        SIM::Loop loop (100u);
        loop.Connect ("Blah.Blah.Blah.Blah", "Blah");
        ASSERT_FALSE (true);
    }
    catch (std::exception& e) {
        ASSERT_EQ (std::string ("Cannot connect <Blah.Blah.Blah.Blah> to <Blah>: Illegal path <Blah.Blah.Blah.Blah>"), e.what ());
    }
}

TEST (SimLoopTester, NonExistingSourceBlocks) {
    
    try {
        SIM::Loop loop (100u);
        loop.AddBlock (std::make_unique<TestBlock> ("Block"));
        loop.Connect ("IDoNotExist.out.output", "Block.in.input");
        ASSERT_FALSE (true);
    }
    catch (std::exception& e) {
        ASSERT_EQ (std::string ("Cannot connect <IDoNotExist.out.output> to <Block.in.input>: Non-existing block <IDoNotExist>"), e.what ());
    }
}

TEST (SimLoopTester, NonExistingTargetBlocks) {
    
    try {
        SIM::Loop loop (100u);
        loop.AddBlock (std::make_unique<TestBlock> ("Block"));
        loop.Connect ("Block.out.output", "IDoNotExist.in.input");
        ASSERT_FALSE (true);
    }
    catch (std::exception& e) {
        ASSERT_EQ (std::string ("Cannot connect <Block.out.output> to <IDoNotExist.in.input>: Non-existing block <IDoNotExist>"), e.what ());
    }
}

TEST (SimLoopTester, RunSuccesfulConfig) {
    
    const std::vector<std::string> names = {
        "block1",
        "block2",
        "block3",
        "block4",
        "block5"
    };
    
    SIM::Loop loop (100u);
    for (const auto& name : names) {
        
        auto block = std::make_unique<TestBlock> (name);
        
        EXPECT_CALL (*block, Initialize (0.0)).Times (1);
        EXPECT_CALL (*block, Step (0.1)).Times (1);
        EXPECT_CALL (*block, Step (0.2)).Times (1);
        EXPECT_CALL (*block, Terminate (0.2)).Times (1);
        
        loop.AddBlock (std::move (block));
    }
    
    loop.Connect ("block1.out.output", "block2.in.input");
    loop.Connect ("block2.out.output", "block3.in.input");
    loop.Connect ("block3.out.output", "block4.in.input");
    loop.Connect ("block4.out.output", "block5.in.input");
    
    loop.Initialize ();
    ASSERT_EQ (0u, loop.GetTimeStamp ());
    
    loop.Update ();
    ASSERT_EQ (100u, loop.GetTimeStamp ());
    
    loop.Update ();
    ASSERT_EQ (200u, loop.GetTimeStamp ());
    
    // loop.Terminate () called in destructor
}

TEST (SimLoopTester, GetPortValue) {
    
    SIM::Loop loop (100u);
    loop.AddBlock (std::make_unique<TestBlock> ("Test"));
    EXPECT_EQ ("0", loop.GetValue ("Test.in.input"));
}

TEST (SimLoopTester, IllegalPath) {
    
    try {
        SIM::Loop loop (100u);
        loop.GetValue ("Test.in.input");
        ASSERT_FALSE (true);
    }
    catch (std::exception& e) {
        ASSERT_EQ (std::string ("Non-existing block <Test>"), e.what ());
    }
}
