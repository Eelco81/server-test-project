
#include "gtest/gtest.h"

#include "SimConnector.h"
#include "MathMatrix.h"
#include "MathVector.h"

#include <vector>
#include <memory>
#include <cstdint>

template <typename T> 
class SimConnectorTester: public ::testing::Test {}; 
using DataTypes = ::testing::Types<bool, int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t, float, double>;
TYPED_TEST_CASE (SimConnectorTester, DataTypes);

TYPED_TEST (SimConnectorTester, Transfer) {
    
    TypeParam value1 (1);
    TypeParam value2 (0);
    
    auto port1 = std::make_shared<SIM::TypedPort<TypeParam>> ("port1", &value1);
    auto port2 = std::make_shared<SIM::TypedPort<TypeParam>> ("port2", &value2);
    
    SIM::Connector connector (port1, port2);
    
    ASSERT_EQ (port1->Get (), value1);
    ASSERT_EQ (port2->Get (), value2);
    
    connector.Transfer ();
    
    ASSERT_EQ (port1->Get (), value1);
    ASSERT_EQ (port2->Get (), value2);
    
    ASSERT_EQ (value1, (TypeParam)1);
    ASSERT_EQ (value2, (TypeParam)1);
}

TYPED_TEST (SimConnectorTester, NonExistingSourcePorts) {
    
    auto port1 = std::shared_ptr<SIM::TypedPort<TypeParam>> (nullptr);
    auto port2 = std::make_shared<SIM::TypedPort<TypeParam>> ("port2");
    
    try {
        SIM::Connector connector (port1, port2);
    }
    catch (std::exception& e) {
        ASSERT_EQ (std::string ("Cannot connect ports, non existing source port"), e.what ());
        return;
    }
    
    ASSERT_FALSE (true);
}

TYPED_TEST (SimConnectorTester, NonExistingTargetPorts) {
    
    auto port1 = std::make_shared<SIM::TypedPort<TypeParam>> ("port1");
    auto port2 = std::shared_ptr<SIM::TypedPort<TypeParam>> (nullptr);
    
    try {
        SIM::Connector connector (port1, port2);
    }
    catch (std::exception& e) {
        ASSERT_EQ (std::string ("Cannot connect ports, non existing target port"), e.what ());
        return;
    }
    
    ASSERT_FALSE (true);
}

TEST (SimConnectorTester, NonMatchingTypes) {
    
    auto port1 = std::make_shared<SIM::TypedPort<bool>> ("port1");
    auto port2 = std::make_shared<SIM::TypedPort<double>> ("port2");

    try {
        SIM::Connector connector (port1, port2);
    }
    catch (std::exception& e) {
        ASSERT_EQ (std::string ("Cannot connect port1 to port2, the types do not match"), e.what ());
        return;
    }
    
    ASSERT_FALSE (true);
}


TEST (SimConnectorTester, NonSupportedTypes) {
    
    auto port1 = std::make_shared<SIM::TypedPort<MATH::Matrix<2,2>>> ("port1");
    auto port2 = std::make_shared<SIM::TypedPort<MATH::Matrix<2,2>>> ("port2");

    try {
        SIM::Connector connector (port1, port2);
    }
    catch (std::exception& e) {
        ASSERT_EQ (std::string ("Cannot connect port1 to port2, unknown port type"), e.what ());
        return;
    }
    
    ASSERT_FALSE (true);
}

TEST (SimConnectorTester, ListedConnectors) {
    
    bool b1 (true);
    bool b2 (false);
    
    auto bool1 = std::make_shared<SIM::TypedPort<bool>> ("bool1", &b1);
    auto bool2 = std::make_shared<SIM::TypedPort<bool>> ("bool2", &b2);
    
    uint32_t u1 (1u);
    uint32_t u2 (0u);
    
    auto uint32_1 = std::make_shared<SIM::TypedPort<uint32_t>> ("uint32_1", &u1);
    auto uint32_2 = std::make_shared<SIM::TypedPort<uint32_t>> ("uint32_2", &u2);
    
    double d1 (1.0);
    double d2 (0.0);
    
    auto double1 = std::make_shared<SIM::TypedPort<double>> ("double1", &d1);
    auto double2 = std::make_shared<SIM::TypedPort<double>> ("double2`", &d2);
    
    std::vector<std::unique_ptr<SIM::Connector>> connectors;
    connectors.emplace_back (std::make_unique<SIM::Connector> (bool1, bool2));
    connectors.emplace_back (std::make_unique<SIM::Connector> (uint32_1, uint32_2));
    connectors.emplace_back (std::make_unique<SIM::Connector> (double1, double2));
    
    ASSERT_EQ (b2, false);
    ASSERT_EQ (u2, 0u);
    ASSERT_EQ (d2, 0.0);
    
    for (auto& connector : connectors) {
        connector->Transfer ();
    }
    
    ASSERT_EQ (b2, true);
    ASSERT_EQ (u2, 1u);
    ASSERT_EQ (d2, 1.0);
}
