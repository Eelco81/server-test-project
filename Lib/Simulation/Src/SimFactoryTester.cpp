
#include <exception>
#include "gmock/gmock.h"

#include "SimBlock.h"
#include "SimLoop.h"
#include "SimFactory.h"

namespace {

class TestBlock : public SIM::Block {
public: 
    TestBlock (const std::string& inName) : 
        Block (inName)
    {
        AddInPort<bool> ("input");
        AddOutPort<bool> ("output");
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
        return block;
    }
};

} // end anonymous namespace

using TestParam = std::tuple<std::string, std::string>;
class SimFactoryTester : public ::testing::TestWithParam<TestParam> {};

INSTANTIATE_TEST_CASE_P (SimFactoryTester, SimFactoryTester,
    ::testing::Values(
        std::make_tuple (R"({})", std::string ("No blocks defined in config")),
        std::make_tuple (R"({ "blocks" : [] })", std::string ("No blocks defined in config")),
        std::make_tuple (R"({ "blocks" : [{}] })", std::string ("Block without \"name\" element in config")),
        std::make_tuple (R"({ "blocks" : [{ "name": "MyName" }] })", std::string ("Block without \"type\" element in config")),
        std::make_tuple (R"({
            "blocks" : [{
                "name" : "MyName",
                "type" : "MyType"
            },{
                "name" : "MyName",
                "type" : "MyType",
                "settings" : {}
            }]
        })", std::string ("Cannot add block <MyName>, the name is not unique")),
        std::make_tuple (R"({
            "blocks" : [{
                "name" : "MyName",
                "type" : "MyType"
            }],
            "connectors":[{}]
        })", std::string ("Connector without \"source\" element in config")),
        std::make_tuple (R"({
            "blocks" : [{
                "name" : "MyName",
                "type" : "MyType"
            }],
            "connectors":[{ "source": "MySource" }]
        })", std::string ("Connector without \"target\" element in config")),
        std::make_tuple (R"({
            "blocks" : [{
                "name" : "MyName",
                "type" : "MyType"
            }],
            "connectors":[{ "source": "Blah.out.output", "target": "MyTarget" }]
        })", std::string ("Cannot connect <Blah.out.output> to <MyTarget>: Non-existing block <Blah>")),
        std::make_tuple (R"({
            "blocks" : [{
                "name" : "MyName",
                "type" : "MyType"
            }],
            "connectors":[{ "source": "MyName.out.output", "target": "Blah.in.input" }]
        })", std::string ("Cannot connect <MyName.out.output> to <Blah.in.input>: Non-existing block <Blah>"))
    )
);

TEST_P (SimFactoryTester, ErronuousConfig) {
    auto config = json::parse (std::get<0> (GetParam ()));
    TestFactory factory;
    try {
        auto loop = factory.Create (config);
        ASSERT_FALSE (true);
    }
    catch (std::exception& e) {
        ASSERT_EQ (std::get<1> (GetParam ()), e.what ());
    }
}
