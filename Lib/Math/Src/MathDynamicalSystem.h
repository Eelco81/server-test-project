
#ifndef _MATH_DYNAMICALSYSTEM_H_
#define _MATH_DYNAMICALSYSTEM_H_

#include "MathMatrix.h"
#include "MathVector.h"

namespace MATH {

template <std::size_t N, std::size_t M, std::size_t K, typename T = double>
class DynSys {
    
public:
    /**
     * System size off_type
     *
     * x'(t) = A x(t) + B u(t)
     *  y(t) = C x(t) + D u(t)
     */
    using A_t = Matrix<N,N,T>;
    using B_t = Matrix<N,M,T>;
    using C_t = Matrix<K,N,T>;
    using D_t = Matrix<K,M,T>;
    using X_t = Vector<N,T>;
    using U_t = Vector<M,T>;
    using Y_t = Vector<K,T>;
    
    /**
     * Constructor
     */
    DynSys ();
    
    /**
     * Constructor
     */
    DynSys (const A_t& inA, const B_t& inB, const C_t& inC, const D_t& inD, T inDT, const X_t& inX = (T)0);
    
    /**
     * Destructor
     */
    virtual ~DynSys ();
    
    /**
     * Reset
     */
    void Reset (const A_t& inA, const B_t& inB, const C_t& inC, const D_t& inD, T inDT, const X_t& inX = (T)0);
    
    /**
     * Update the system with u(t)
     */
    void Update (const U_t& inU);
    
    /**
     * Retrieve the system output y(t)
     */
    Y_t& OutputRef () { return mY; }
    
private:
    A_t mA;
    B_t mB;
    C_t mC;
    D_t mD;
    X_t mX;
    Y_t mY;
    T mDT;
};

}

#include "MathDynamicalSystem.tcc"

#endif // _MATH_DYNAMICALSYSTEM_H_
