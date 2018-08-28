#include <algorithm>

template<std::size_t Nrows, std::size_t Ncols, typename T>
MATH::Matrix<Nrows,Ncols,T>::Matrix () :
    Matrix (static_cast<T> (0))
{
}

template<std::size_t Nrows, std::size_t Ncols, typename T>
MATH::Matrix<Nrows,Ncols,T>::Matrix (T inValue) :
    mData (Nrows * Ncols, inValue)
{
}

template<std::size_t Nrows, std::size_t Ncols, typename T>
MATH::Matrix<Nrows,Ncols,T>::Matrix (const MATH::Matrix<Nrows,Ncols,T>& inMatrix) {
    std::copy (inMatrix.mData.begin (), inMatrix.mData.end (), std::back_inserter (mData));
}

template<std::size_t Nrows, std::size_t Ncols, typename T>
MATH::Matrix<Nrows,Ncols,T>::~Matrix () = default;

template<std::size_t Nrows, std::size_t Ncols, typename T>
T MATH::Matrix<Nrows,Ncols,T>::operator() (std::size_t inRow, std::size_t inCol) const {
    return mData[inRow + inCol * Nrows];
}

template<std::size_t Nrows, std::size_t Ncols, typename T>
T& MATH::Matrix<Nrows,Ncols,T>::operator() (std::size_t inRow, std::size_t inCol) {
    return mData[inRow + inCol * Nrows];
}

template<std::size_t Nrows, std::size_t Ncols, typename T>
MATH::Matrix<Nrows,Ncols,T> MATH::Matrix<Nrows,Ncols,T>::operator+ (const MATH::Matrix<Nrows,Ncols,T>& inMatrix) const {

    Matrix mat;
    
    for (std::size_t i (0u); i < Nrows; i++) {
        for (std::size_t j (0u); j < Ncols; j++) {
            mat(i,j) = (*this)(i,j) + inMatrix(i,j);
        }
    }
    
    return mat;
}

template<std::size_t Nrows, std::size_t Ncols, typename T>
MATH::Matrix<Nrows,Ncols,T> MATH::Matrix<Nrows,Ncols,T>::operator+ (T inValue) const {

    Matrix mat;
    
    for (std::size_t i (0u); i < Nrows; i++) {
        for (std::size_t j (0u); j < Ncols; j++) {
            mat(i,j) = (*this)(i,j) + inValue;
        }
    }
    
    return mat;
}

template<std::size_t Nrows, std::size_t Ncols, typename T>
MATH::Matrix<Nrows,Ncols,T> MATH::Matrix<Nrows,Ncols,T>::operator- (const MATH::Matrix<Nrows,Ncols,T>& inMatrix) const {
 
    Matrix mat;
    
    for (std::size_t i (0u); i < Nrows; i++) {
        for (std::size_t j (0u); j < Ncols; j++) {
            mat(i,j) = (*this)(i,j) - inMatrix(i,j);
        }
    }
    
    return mat;
}

template<std::size_t Nrows, std::size_t Ncols, typename T>
MATH::Matrix<Nrows,Ncols,T> MATH::Matrix<Nrows,Ncols,T>::operator- (T inValue) const {
    return *this + (-inValue);
}

template<std::size_t Nrows, std::size_t Ncols, typename T>
MATH::Matrix<Nrows,Ncols,T> MATH::Matrix<Nrows,Ncols,T>::operator* (T inValue) const {

    Matrix mat;
    
    for (std::size_t i (0u); i < Nrows; i++) {
        for (std::size_t j (0u); j < Ncols; j++) {
            mat(i,j) = (*this)(i,j) * inValue;
        }
    }
    
    return mat;
}

template<std::size_t Nrows, std::size_t Ncols, typename T>
MATH::Matrix<Nrows,Ncols,T> MATH::Matrix<Nrows,Ncols,T>::operator/ (T inValue) const {

    Matrix mat;
    
    for (std::size_t i (0u); i < Nrows; i++) {
        for (std::size_t j (0u); j < Ncols; j++) {
            mat(i,j) = (*this)(i,j) / inValue;
        }
    }
    
    return mat;
}

template<std::size_t Nrows, std::size_t Ncols, typename T>
bool MATH::Matrix<Nrows,Ncols,T>::operator== (T inValue) const {
    return std::all_of (mData.cbegin (), mData.cend (), [inValue](const T& value) { return value == inValue; });
}

template<std::size_t Nrows, std::size_t Ncols, typename T>
bool MATH::Matrix<Nrows,Ncols,T>::operator== (const MATH::Matrix<Nrows,Ncols,T>& inMatrix) const {
    return std::equal (mData.cbegin (), mData.cend (), inMatrix.mData.cbegin ());
}
