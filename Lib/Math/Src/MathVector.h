
#ifndef _MATH_VECTOR_H_
#define _MATH_VECTOR_H_

#include <iostream>
#include "MathMatrix.h"

namespace MATH {

template <std::size_t Nrows, typename T = double>
using Vector = Matrix<Nrows,1,T>;

template <std::size_t Nrows, typename T>
T operator^ (const Vector<Nrows,T>& inLhs, const Vector<Nrows,T>& inRhs) {
    
    T result (0);
    for (std::size_t i (0u); i < Nrows; i++) {
        result += inLhs(i) * inRhs (i);
    }
    return result;
}

}

#endif //  _MATH_VECTOR_H_
