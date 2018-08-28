
#ifndef _MATH_MATRIX_H_
#define _MATH_MATRIX_H_

#include <vector>

namespace MATH {

template <std::size_t Nrows, std::size_t Ncols, typename T = double>
class Matrix {

public:
    Matrix ();
    Matrix (T inValue);
    Matrix (const Matrix& inMatrix);
    virtual ~Matrix ();
    
public:
    T Get (std::size_t inRow, std::size_t inCol) const;
    
public:
    T operator() (std::size_t inRow, std::size_t inCol) const;
    T& operator() (std::size_t inRow, std::size_t inCol);
    
    Matrix operator+ (const Matrix& inMatrix) const;
    Matrix operator+ (T inValue) const;
    Matrix operator- (const Matrix& inMatrix) const;
    Matrix operator- (T inValue) const;
    Matrix operator* (T inValue) const;
    Matrix operator/ (T inValue) const;
    
    bool operator== (const Matrix& inMatrix) const;
    bool operator== (T inValue) const;
    
    
private:
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

}

#include "MathMatrix.tcc"

#endif
