
#include "gtest/gtest.h"
#include "SimInitializer.h"

#include <vector>
#include <cstdint>

template <typename T> 
class SimInitializerTester: public ::testing::Test {}; 
using DataTypes = ::testing::Types<bool, int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t, float, double>;
TYPED_TEST_CASE (SimInitializerTester, DataTypes);

TYPED_TEST (SimInitializerTester, Transfer) {
    
    TypeParam value (1);
    
    auto port = std::make_shared<SIM::TypedPort<TypeParam>> ("port", &value);
    
    SIM::Initializer initializer (port, 0.0);
    ASSERT_EQ (port->Get (), value);
    
    initializer.Apply ();
    ASSERT_EQ (port->Get (), (TypeParam)0);
    ASSERT_EQ (value, (TypeParam)0);
}

TYPED_TEST (SimInitializerTester, NonExistingSourcePorts) {
    
    auto port = std::shared_ptr<SIM::TypedPort<TypeParam>> (nullptr);
    SIM::Initializer initializer (port, 0.0);
    ASSERT_THROW (initializer.Apply (), std::exception);
}