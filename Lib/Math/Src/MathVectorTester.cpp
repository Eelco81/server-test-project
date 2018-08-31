
#include "gtest/gtest.h"
#include "MathVector.h"

template <typename T> 
class VectorTester: public ::testing::Test {}; 
using DataTypes = ::testing::Types<int8_t, int16_t, int32_t, uint32_t, float, double>;
TYPED_TEST_CASE (VectorTester, DataTypes);

TYPED_TEST (VectorTester, GetAndSet) {
    
    MATH::Vector<10,TypeParam> a (1);
    
    ASSERT_EQ (a(0), (TypeParam)1);
    ASSERT_EQ (a, (TypeParam)1);
    
    a(0) = 10;
    
    ASSERT_EQ (a(0), (TypeParam)10);
    ASSERT_FALSE (a == (TypeParam)1);
}

TYPED_TEST (VectorTester, DotProduct) {
    
    MATH::Vector<10,TypeParam> a (1);
    MATH::Vector<10,TypeParam> b (2);
    
    ASSERT_EQ (a, (TypeParam)1);
    ASSERT_EQ (b, (TypeParam)2);
    
    const auto dot (a ^ b);
    ASSERT_EQ (dot, (TypeParam)20);
}

TYPED_TEST (VectorTester, MatrixVectorMultiplication) {
    
    MATH::Matrix<10u,5u,TypeParam> A (1);
    MATH::Vector<5u,TypeParam> b (2);
    
    const MATH::Vector<10u,TypeParam> result (A * b);
    ASSERT_EQ (result, (TypeParam)10);
}

TEST (VectorTester, MatrixVectorSolver) {
    
    MATH::Matrix<2,2> A;
    A(0,0) = 1.0;
    A(1,1) = 2.0;
    
    const MATH::Vector<2> b (2);
    
    const auto c (A / b);
    
}