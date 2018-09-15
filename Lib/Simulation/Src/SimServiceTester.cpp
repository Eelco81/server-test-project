
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
    }
    MOCK_METHOD1 (Configure, void (const json&));
    MOCK_METHOD0 (Initialize, void ());
    MOCK_METHOD0 (Step, void ());
    MOCK_METHOD0 (Terminate, void ());
};

class TestFactory : public SIM::Factory {
protected :
    std::unique_ptr<SIM::Block> CreateBlock (const std::string& inName, const std::string& inType) override {
        auto block = std::make_unique<TestBlock> (inName);
        EXPECT_CALL (*block, Configure (::testing::_)).Times (1);
        EXPECT_CALL (*block, Initialize ()).Times (1);
        EXPECT_CALL (*block, Step ()).Times (2);
        EXPECT_CALL (*block, Terminate ()).Times (1);
        return block;
    }
};

} // end anonymous namespace

TEST (SimServiceTester, Run) {
    
    auto factory = std::make_unique<TestFactory> ();
    SIM::Service service (std::move (factory));
    
    auto config = R"({
        "blocks" : [{
            "name" : "MyName", 
            "type" : "MyType"
        }]
    })"_json;
    
    ASSERT_TRUE (service.Load (config));
    ASSERT_TRUE (service.Start ());
    
    OS::Timing::Sleep (150u);
    
    ASSERT_TRUE (service.Stop ());
}
