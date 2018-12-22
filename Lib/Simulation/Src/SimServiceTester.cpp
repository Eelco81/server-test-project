
#include "gmock/gmock.h"
#include "Timing.h"
#include "SimValue.h"
#include "SimBlock.h"
#include "SimFactory.h"
#include "SimService.h"

namespace {

class TestBlock : public SIM::Block {
public: 
    TestBlock (const std::string& inName) : 
        Block (inName)
    {
        AddPort<uint8_t> (SIM::Path::INPUT, "input");
    }
    MOCK_METHOD1 (Configure, void (const json&));
    MOCK_METHOD2 (Initialize, void (double, double));
    MOCK_METHOD2 (Step, void (double, double));
    MOCK_METHOD2 (Terminate, void (double, double));
};

class TestFactory : public SIM::Factory {
protected :
    std::unique_ptr<SIM::Block> CreateBlock (const std::string& inName, const std::string& inType) override {
        auto block = std::make_unique<TestBlock> (inName);
        EXPECT_CALL (*block, Configure (::testing::_)).Times (1);
        EXPECT_CALL (*block, Initialize (0.0, 0.1)).Times (1);
        EXPECT_CALL (*block, Step (0.1, 0.1)).Times (1);
        EXPECT_CALL (*block, Step (0.2, 0.1)).Times (1);
        EXPECT_CALL (*block, Terminate (0.2, 0.1)).Times (1);
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
    
    ASSERT_THROW (service.Start (), std::exception);
    ASSERT_THROW (service.Stop (), std::exception);
    
    service.Load (config);
    
    ASSERT_THROW (service.Stop (), std::exception);
    
    service.Start ();
    
    ASSERT_THROW (service.Start (), std::exception);
    ASSERT_THROW (service.Load (config), std::exception);
    
    OS::Timing::Sleep (150u);
    
    EXPECT_EQ (0.0, service.GetValue (SIM::Path ("MyName", "input", SIM::Path::INPUT)).mValue);
    
    const auto paths = service.GetPaths ();
    ASSERT_EQ (std::string ("MyName.in.input"), paths[0].ToString ());
    
    service.Stop ();
    
    ASSERT_THROW (service.Stop (), std::exception);
}
