
#ifndef _MATH_MATRIX_H_
#define _MATH_MATRIX_H_

#include <vector>

namespace MATH {

template <std::size_t Nrows, std::size_t Ncols, typename T = double>
class Matrix {

public:
    /**
     * Constructors.
     */
    Matrix ();
    Matrix (T inValue);
    Matrix (const Matrix& inMatrix);
    
    /**
     * Destructor.
     */
    virtual ~Matrix ();
    
    /**
     * Access operators.
     */
    T operator() (std::size_t inRow, std::size_t inCol = 0u) const;
    T& operator() (std::size_t inRow, std::size_t inCol = 0u);
    T* GetValuePtr (std::size_t inRow, std::size_t inCol = 0u);
    
    /**
     * Assignment operators.
     */
    Matrix& operator= (const Matrix& inMatrix);
    Matrix& operator= (T inValue);
    
    /**
     * Arithmic operators.
     */
    Matrix operator+ (const Matrix& inMatrix) const;
    Matrix operator+ (T inValue) const;
    Matrix operator- (const Matrix& inMatrix) const;
    Matrix operator- (T inValue) const;
    Matrix operator* (T inValue) const;
    Matrix operator/ (T inValue) const;
    
    /**
     * Transpose.
     */
    Matrix<Ncols,Nrows,T> operator~ () const;
    
    /**
     * Equality check operators.
     */
    bool operator== (const Matrix& inMatrix) const;
    bool operator== (T inValue) const;
    
protected:
    std::vector<T> mData;
};

template <std::size_t M, std::size_t K, std::size_t N, typename T = double>
Matrix<M,N,T> operator* (const Matrix<M,K,T>& inLhs, const Matrix<K,N,T>& inRhs) {
    
    Matrix<M,N,T> mat (0);
    for (std::size_t i (0u); i < M; i++) {
        for (std::size_t j (0u); j < N; j++) {
            for (std::size_t k (0u); k < K; k++) {
                mat(i,j) += inLhs(i,k) * inRhs(k,j);
            }
        }
    }
    return mat;
}

template <std::size_t N, std::size_t K, typename T = double>
Matrix<N,K,T> operator/ (const Matrix<N,N,T>& inLhs, const Matrix<N,K,T>& inRhs) {
    
    auto Q (inLhs);
    
    Matrix<N,K,T> mat;
    return mat;
}

template <std::size_t N, typename T = double>
Matrix<N,N,T> Diag (T inValue, int64_t inIndex) {
    MATH::Matrix<N,N,T> mat;
    for (std::size_t i(0u); i < N - std::abs (inIndex); i++) {
        if (inIndex >=0) {
            mat(i, i + std::abs (inIndex)) = inValue;
        }
        else {
            mat(i + std::abs(inIndex), i) = inValue;
        }
    }
    return mat;
}

template <std::size_t N, typename T = double>
Matrix<N,N,T> Identity () {
    return Diag<N,T> ((T)1, 0);
}

template <std::size_t N,typename T = double>
Matrix<N*2,N*2,T> Augment (MATH::Matrix<N,N,T> A1, MATH::Matrix<N,N,T> A2, MATH::Matrix<N,N,T> A3, MATH::Matrix<N,N,T> A4) {
    MATH::Matrix<N*2,N*2,T> A;
    for (std::size_t i(0u); i < N; i++) {
        for (std::size_t j(0u); j < N; j++) {
            A(i  ,j  ) = A1(i,j);
            A(i  ,j+N) = A2(i,j);
            A(i+N,j  ) = A3(i,j);
            A(i+N,j+N) = A4(i,j);
        }
    }
    return A;
}

}

#include "MathMatrix.tcc"

#endif
