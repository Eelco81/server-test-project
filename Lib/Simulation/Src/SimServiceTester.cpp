
#include "gmock/gmock.h"

#include "Timing.h"

#include "SimBlock.h"
#include "SimFactory.h"
#include "SimService.h"

namespace {

class TestBlock : public SIM::Block {
public: 
    TestBlock (const std::string& inName) : 
        Block (inName)
    {
        AddInPort<uint8_t> ("input");
    }
    MOCK_METHOD1 (Configure, void (const json&));
    MOCK_METHOD1 (Initialize, void (double));
    MOCK_METHOD1 (Step, void (double));
    MOCK_METHOD1 (Terminate, void (double));
};

class TestFactory : public SIM::Factory {
protected :
    std::unique_ptr<SIM::Block> CreateBlock (const std::string& inName, const std::string& inType) override {
        auto block = std::make_unique<TestBlock> (inName);
        EXPECT_CALL (*block, Configure (::testing::_)).Times (1);
        EXPECT_CALL (*block, Initialize (0.0)).Times (1);
        EXPECT_CALL (*block, Step (0.1)).Times (1);
        EXPECT_CALL (*block, Step (0.2)).Times (1);
        EXPECT_CALL (*block, Terminate (0.2)).Times (1);
        return block;
    }
};

} // end anonymous namespace

TEST (SimServiceTester, Run) {
    
    auto factory = std::make_unique<TestFactory> ();
    SIM::Service service (std::move (factory));
    
    const auto config = R"({
        "step": 100,
        "blocks" : [{
            "name" : "MyName", 
            "type" : "MyType"
        }]
    })"_json;
    
    ASSERT_TRUE (service.Load (config));
    ASSERT_TRUE (service.Start ());
    
    OS::Timing::Sleep (199u);
    
    std::string value;
    ASSERT_TRUE (service.GetValue ("MyName.in.input", value));
    EXPECT_EQ (std::string ("0"), value);
    
    ASSERT_TRUE (service.Stop ());
}
