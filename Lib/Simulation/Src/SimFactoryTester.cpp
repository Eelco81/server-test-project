
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
        std::make_tuple (R"({})", std::string ("No valid step defined in config")),
        std::make_tuple (R"({ "step": "blah" })", std::string ("No valid step defined in config")),  
        std::make_tuple (R"({ "step": 10 })", std::string ("No blocks defined in config")),
        std::make_tuple (R"({ "step": 10, "blocks" : [] })", std::string ("No blocks defined in config")),
        std::make_tuple (R"({ "step": 10, "blocks" : [{}] })", std::string ("Block without \"name\" element in config")),
        std::make_tuple (R"({ "step": 10, "blocks" : [{ "name": "MyName" }] })", std::string ("Block without \"type\" element in config")),
        std::make_tuple (R"({
            "step": 10,
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
            "step": 10,
            "blocks" : [{
                "name" : "MyName",
                "type" : "MyType"
            }],
            "connectors":[{}]
        })", std::string ("Connector without \"source\" element in config")),
        std::make_tuple (R"({
            "step": 10,
            "blocks" : [{
                "name" : "MyName",
                "type" : "MyType"
            }],
            "connectors":[{ "source": "MySource" }]
        })", std::string ("Connector without \"target\" element in config")),
        std::make_tuple (R"({
            "step": 10,
            "blocks" : [{
                "name" : "MyName",
                "type" : "MyType"
            }],
            "connectors":[{ "source": "Blah.out.output", "target": "MyName.in.input" }]
        })", std::string ("Cannot connect <Blah.out.output> to <MyName.in.input>: Non-existing block <Blah>")),
        std::make_tuple (R"({
            "step": 10,
            "blocks" : [{
                "name" : "MyName",
                "type" : "MyType"
            }],
            "connectors":[{ "source": "MyName.out.output", "target": "Blah.in.input" }]
        })", std::string ("Cannot connect <MyName.out.output> to <Blah.in.input>: Non-existing block <Blah>")),
        std::make_tuple (R"({
            "step": 10,
            "blocks" : [{
                "name" : "MyName",
                "type" : "MyType"
            }],
            "sample" : [
                12345
            ]
        })", std::string ("Sample path is not a string")),
        std::make_tuple (R"({
            "step": 10,
            "blocks" : [{
                "name" : "MyName",
                "type" : "MyType"
            }],
            "sample" : [
                "I do not exist"
            ]
        })", std::string ("Cannot sample <I do not exist>: Illegal path <I do not exist>")),
        std::make_tuple (R"({
            "step": 10,
            "blocks" : [{
                "name" : "MyName",
                "type" : "MyType"
            }],
            "initializers" : [
                {}
            ]
        })", std::string ("Initializer without \"port\" element in config")),
        std::make_tuple (R"({
            "step": 10,
            "blocks" : [{
                "name" : "MyName",
                "type" : "MyType"
            }],
            "initializers" : [
                { "port" : "someport" }
            ]
        })", std::string ("Initializer without \"value\" element in config")),
        std::make_tuple (R"({
            "step": 10,
            "blocks" : [{
                "name" : "MyName",
                "type" : "MyType"
            }],
            "initializers" : [
                { "port" : "I do not exist", "value": 1 }
            ]
        })", std::string ("Cannot initialize <I do not exist>: Illegal path <I do not exist>"))
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
