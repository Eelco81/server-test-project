
#include <cstdint>

#include "gtest/gtest.h"

#include "SimTypedPort.h"

template <typename T> 
class SimPortTester: public ::testing::Test {}; 
using DataTypes = ::testing::Types<bool, int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t, float, double>;
TYPED_TEST_CASE (SimPortTester, DataTypes);

TYPED_TEST (SimPortTester, Name) {
    SIM::TypedPort<TypeParam> port (SIM::Path ("test.in.port"));
    EXPECT_EQ (std::string ("test.in.port"), port.GetPath ().ToString ());
}

TYPED_TEST (SimPortTester, Info) {
    SIM::TypedPort<TypeParam> port (SIM::Path ("test.in.port"));
    EXPECT_EQ (std::string ("test.in.port"), port.GetInfo ().mPath.ToString ());
}

TYPED_TEST (SimPortTester, DefaultConstructor) {
    SIM::TypedPort<TypeParam> port (SIM::Path ("test.in.port"));
    EXPECT_EQ (port.Get (), (TypeParam)0);
}

TYPED_TEST (SimPortTester, Set) {
    SIM::TypedPort<TypeParam> port (SIM::Path ("test.in.port"));
    port.Set ((TypeParam)1);
    EXPECT_EQ (port.Get (), (TypeParam)1);
}

TYPED_TEST (SimPortTester, Assignment) {
    SIM::TypedPort<TypeParam> port1 (SIM::Path ("test.in.port1"));
    SIM::TypedPort<TypeParam> port2 (SIM::Path ("test.in.port2"));
    port1.Set ((TypeParam)1);
    port2 = port1;
    EXPECT_EQ (port1.Get (), (TypeParam)1);
    EXPECT_EQ (port2.Get (), (TypeParam)1);
}

TYPED_TEST (SimPortTester, ValueAssignment) {
    SIM::TypedPort<TypeParam> port (SIM::Path ("test.in.port"));
    port = (TypeParam)1;
    EXPECT_EQ (port.Get (), (TypeParam)1);
}

TYPED_TEST (SimPortTester, StringValue) {
    SIM::TypedPort<TypeParam> port (SIM::Path ("test.in.port"));
    EXPECT_NE (port.GetStringValue (), "");
    port.SetStringValue ("1");
    EXPECT_NE (port.GetStringValue (), "");
}

TYPED_TEST (SimPortTester, StringValueInvalidString) {
    SIM::TypedPort<TypeParam> port (SIM::Path ("test.in.port"));
    try {
        port.SetStringValue ("blah");
        ASSERT_TRUE (false);
    }
    catch (std::exception& e) {
        ASSERT_EQ (std::string ("Invalid value string for port <test.in.port>"), e.what());
    }
}

TYPED_TEST (SimPortTester, NumericValue) {
    SIM::TypedPort<TypeParam> port (SIM::Path ("test.in.port"));
    EXPECT_EQ (port.GetNumericValue (), 0.0);
    port.SetNumericValue (1.0);
    EXPECT_EQ (port.GetNumericValue (), 1.0);
}

TEST (SimPortTester, TypeBool) {
    ASSERT_EQ (SIM::Port::BOOL, SIM::TypedPort<bool> (SIM::Path ("test.in.port")).GetType ());
}

TEST (SimPortTester, TypeUint8) {
    ASSERT_EQ (SIM::Port::UINT8, SIM::TypedPort<uint8_t> (SIM::Path ("test.in.port")).GetType ());
}

TEST (SimPortTester, TypeUint16) {
    ASSERT_EQ (SIM::Port::UINT16, SIM::TypedPort<uint16_t> (SIM::Path ("test.in.port")).GetType ());
}

TEST (SimPortTester, TypeUint32) {
    ASSERT_EQ (SIM::Port::UINT32, SIM::TypedPort<uint32_t> (SIM::Path ("test.in.port")).GetType ());
}

TEST (SimPortTester, TypeUint64) {
    ASSERT_EQ (SIM::Port::UINT64, SIM::TypedPort<uint64_t> (SIM::Path ("test.in.port")).GetType ());
}

TEST (SimPortTester, TypeInt8) {
    ASSERT_EQ (SIM::Port::INT8, SIM::TypedPort<int8_t> (SIM::Path ("test.in.port")).GetType ());
}

TEST (SimPortTester, TypeInt16) {
    ASSERT_EQ (SIM::Port::INT16, SIM::TypedPort<int16_t> (SIM::Path ("test.in.port")).GetType ());
}

TEST (SimPortTester, TypeInt32) {
    ASSERT_EQ (SIM::Port::INT32, SIM::TypedPort<int32_t> (SIM::Path ("test.in.port")).GetType ());
}

TEST (SimPortTester, TypeInt64) {
    ASSERT_EQ (SIM::Port::INT64, SIM::TypedPort<int64_t> (SIM::Path ("test.in.port")).GetType ());
}

TEST (SimPortTester, TypeFloat) {
    ASSERT_EQ (SIM::Port::FLOAT, SIM::TypedPort<float> (SIM::Path ("test.in.port")).GetType ());
}

TEST (SimPortTester, TypeDouble) {
    ASSERT_EQ (SIM::Port::DOUBLE, SIM::TypedPort<double> (SIM::Path ("test.in.port")).GetType ());
}
