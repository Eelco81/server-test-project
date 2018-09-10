
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
        return std::make_unique<TestBlock> (inName);
    }
};

} // end anonymous namespace

TEST (SimFactoryTester, NoBlocks) {
    try {
        const auto config = R"({})"_json;
        TestFactory factory;
        auto loop = factory.Create (config);
        ASSERT_FALSE (true);
    }
    catch (std::exception& e) {
        ASSERT_EQ (std::string ("No blocks defined in config"), e.what ());
    }
}

TEST (SimFactoryTester, EmptyList) {
    try {
        const auto config = R"({ "blocks" : [] })"_json;
        TestFactory factory;
        auto loop = factory.Create (config);
        ASSERT_FALSE (true);
    }
    catch (std::exception& e) {
        ASSERT_EQ (std::string ("No blocks defined in config"), e.what ());
    }
}

TEST (SimFactoryTester, NoName) {
    try {
        const auto config = R"({ "blocks" : [{}] })"_json;
        TestFactory factory;
        auto loop = factory.Create (config);
        ASSERT_FALSE (true);
    }
    catch (std::exception& e) {
        ASSERT_EQ (std::string ("Block without \"name\" element in config"), e.what ());
    }
}

TEST (SimFactoryTester, NoType) {
    try {
        const auto config = R"({ "blocks" : [{"name": "MyName"}] })"_json;
        TestFactory factory;
        auto loop = factory.Create (config);
        ASSERT_FALSE (true);
    }
    catch (std::exception& e) {
        ASSERT_EQ (std::string ("Block without \"type\" element in config"), e.what ());
    }
}

TEST (SimFactoryTester, DoubleNames) {
    try {
        const auto config = R"(
        {
            "blocks" : [{
                "name" : "MyName",
                "type" : "MyType",
                "settings" : {}
            },{
                "name" : "MyName",
                "type" : "MyType",
                "settings" : {}
            }]
        }
        )"_json;
        TestFactory factory;
        auto loop = factory.Create (config);
        ASSERT_FALSE (true);
    }
    catch (std::exception& e) {
        ASSERT_EQ (std::string ("Cannot add block <MyName>, the name is not unique"), e.what ());
    }
}
