
#include "gtest/gtest.h"
#include "MathMatrix.h"

#include <iostream>
#include <cstdint>

template <typename T>
class MathMatrixTester: public ::testing::Test {}; 
using DataTypes = ::testing::Types<int8_t, int16_t, int32_t, uint32_t, float, double>;
TYPED_TEST_CASE (MathMatrixTester, DataTypes);

TYPED_TEST (MathMatrixTester, Constructors) {
    
    const MATH::Matrix<50u,20u,TypeParam>  A;
    const MATH::Matrix<50u,20u,TypeParam>  B (10);
    const auto C (B);
    
    ASSERT_EQ (A, 0);
    ASSERT_EQ (B, 10);
    ASSERT_EQ (C, 10);
    ASSERT_EQ (C, B);
};

TYPED_TEST (MathMatrixTester, SpecialConstructors) {
    
    const auto A = MATH::Diag<3u,TypeParam> ((TypeParam)1, 0);
    const auto B = MATH::Diag<3u,TypeParam> ((TypeParam)1, -1);
    const auto C = MATH::Diag<3u,TypeParam> ((TypeParam)1, 1);
    
    ASSERT_EQ ((TypeParam)1, A(0,0));
    ASSERT_EQ ((TypeParam)0, A(0,1));
    ASSERT_EQ ((TypeParam)0, A(0,2));
    ASSERT_EQ ((TypeParam)0, A(1,0));
    ASSERT_EQ ((TypeParam)1, A(1,1));
    ASSERT_EQ ((TypeParam)0, A(1,2));
    ASSERT_EQ ((TypeParam)0, A(2,0));
    ASSERT_EQ ((TypeParam)0, A(2,1));
    ASSERT_EQ ((TypeParam)1, A(2,2));
    
    ASSERT_EQ ((TypeParam)0, B(0,0));
    ASSERT_EQ ((TypeParam)0, B(0,1));
    ASSERT_EQ ((TypeParam)0, B(0,2));
    ASSERT_EQ ((TypeParam)1, B(1,0));
    ASSERT_EQ ((TypeParam)0, B(1,1));
    ASSERT_EQ ((TypeParam)0, B(1,2));
    ASSERT_EQ ((TypeParam)0, B(2,0));
    ASSERT_EQ ((TypeParam)1, B(2,1));
    ASSERT_EQ ((TypeParam)0, B(2,2));
    
    ASSERT_EQ ((TypeParam)0, C(0,0));
    ASSERT_EQ ((TypeParam)1, C(0,1));
    ASSERT_EQ ((TypeParam)0, C(0,2));
    ASSERT_EQ ((TypeParam)0, C(1,0));
    ASSERT_EQ ((TypeParam)0, C(1,1));
    ASSERT_EQ ((TypeParam)1, C(1,2));
    ASSERT_EQ ((TypeParam)0, C(2,0));
    ASSERT_EQ ((TypeParam)0, C(2,1));
    ASSERT_EQ ((TypeParam)0, C(2,2));
};

TYPED_TEST (MathMatrixTester, Augment) {
    
    const auto A1 = MATH::Matrix<1u,1u,TypeParam> ((TypeParam)1);
    const auto A2 = MATH::Matrix<1u,1u,TypeParam> ((TypeParam)0);
    const auto A3 = MATH::Matrix<1u,1u,TypeParam> ((TypeParam)0);
    const auto A4 = MATH::Matrix<1u,1u,TypeParam> ((TypeParam)1);
    
    const auto A = MATH::Augment (A1, A2, A3, A4);
    ASSERT_EQ ((TypeParam)1, A(0,0));
    ASSERT_EQ ((TypeParam)0, A(0,1));
    ASSERT_EQ ((TypeParam)0, A(1,0));
    ASSERT_EQ ((TypeParam)1, A(1,1));
};

TYPED_TEST (MathMatrixTester, GetSet) {
    
    MATH::Matrix<50u,20u,TypeParam> A (10);
    ASSERT_EQ((TypeParam)10, A(0,0));
    ASSERT_EQ((TypeParam)10, *A.GetValuePtr(1,0));
    
    A(1,0) = 20;
    ASSERT_EQ((TypeParam)20, A(1,0));
    ASSERT_FALSE(A==10);
    ASSERT_EQ((TypeParam)20, *A.GetValuePtr(1,0));
};

TYPED_TEST (MathMatrixTester, Assignment) {
    
    MATH::Matrix<50u,20u,TypeParam> A (1);
    MATH::Matrix<50u,20u,TypeParam> B (0);
    A = B;
    ASSERT_EQ (A, (TypeParam)0);
    ASSERT_EQ (B, (TypeParam)0);
    A = (TypeParam)5;
    ASSERT_EQ (A, (TypeParam)5);
};

TYPED_TEST (MathMatrixTester, Adding) {
    
    const MATH::Matrix<50u,20u,TypeParam> A (1);
    const auto B (A + A + A);
    const auto C (A + 2);
    ASSERT_EQ (B, 3);
    ASSERT_EQ (C, 3);
    ASSERT_EQ (B, C);
};

TYPED_TEST (MathMatrixTester, Subtracting) {
    
    const MATH::Matrix<50u,20u,TypeParam> A (1);
    const auto B (A - A);
    const auto C (A - 1);
    ASSERT_EQ (B, 0);
    ASSERT_EQ (C, 0);
    ASSERT_EQ (B, C);
};

TYPED_TEST (MathMatrixTester, Multiplication) {
    
    const MATH::Matrix<10u,5u,TypeParam> A (1);
    const MATH::Matrix<5u,3u,TypeParam> B (1);

    const MATH::Matrix<10u,3u,TypeParam> C (A * B);
    ASSERT_EQ (C, 5);
    
    const MATH::Matrix<10u,5u,TypeParam> D (A * 5);
    ASSERT_EQ (D, 5);
};

TYPED_TEST (MathMatrixTester, Division) {
    
    const MATH::Matrix<10u,5u,TypeParam> A (10);
    const auto B (A / (TypeParam)2);
    ASSERT_EQ (B, 5);
};

TYPED_TEST (MathMatrixTester, Transpose) {
    
    const MATH::Matrix<10u,5u,TypeParam> A (2);
    
    const MATH::Matrix<5u,10u,TypeParam> B (~A);
    ASSERT_EQ (B, 2);

    const MATH::Matrix<10u,5u,TypeParam> C (2);
    
    const MATH::Matrix<5u,5u,TypeParam> D ((~A)*A);
    ASSERT_EQ (D, 40);
    
    const MATH::Matrix<10u,10u,TypeParam> E (A*(~A));
    ASSERT_EQ (E, 20);
};
