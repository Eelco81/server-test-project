
#include "gtest/gtest.h"
#include "MathDynamicalSystem.h"

TEST (MathDynamicalSystemTester, Oscillator) {
    
    MATH::Matrix<2,2> A (0.0);
    MATH::Matrix<2,1> B (0.0);
    MATH::Matrix<3,2> C (0.0);
    MATH::Matrix<3,1> D (0.0);
    
    const double m (0.05);
    const double d (0.3);
    const double k (1.0);
    
    //
    // m x'' + d x'+ k x = u
    // 
    // [ x, x' ]' = [ 0 1, -k/m -d/m ] [ x, x' ] + [ 0, 1/m ] u
    // [ x, x', x'' ] = [ 1 0, 0 1, -k/m -d/m ] [ x, x' ] + [ 0, 0, 1/m ] u
    // 
    
    A(0,1) = 1.0;
    A(1,0) = -k / m;
    A(1,1) = -d / m;
    B(1,0) = 1.0 / m;
    C(0,0) = 1.0;
    C(1,1) = 1.0;
    C(2,0) = -k / m;
    C(2,1) = -d / m;
    D(2,0) = 1.0 / m; 
    
    MATH::DynSys<2,1,3> sys (A, B, C, D, 0.01);
    
    for (std::size_t i (0u); i < 1000u; i++) {
        sys.Update (-1.0);
    }
    
    ASSERT_NEAR (-1.0, sys.OutputRef ()(0), 0.0001);
    ASSERT_NEAR (0.0, sys.OutputRef ()(1), 0.0001);
    ASSERT_NEAR (0.0, sys.OutputRef ()(2), 0.0001);
    
    for (std::size_t i (0u); i < 1000u; i++) {
        sys.Update (1.0);
    }
    
    ASSERT_NEAR (1.0, sys.OutputRef ()(0), 0.0001);
    ASSERT_NEAR (0.0, sys.OutputRef ()(1), 0.0001);
    ASSERT_NEAR (0.0, sys.OutputRef ()(2), 0.0001);
    
}

