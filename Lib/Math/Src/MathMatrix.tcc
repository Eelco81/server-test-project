#include <algorithm>
#include <functional>

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
MATH::Matrix<Nrows,Ncols,T>::Matrix (const MATH::Matrix<Nrows,Ncols,T>& inMatrix) 
{
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
T* MATH::Matrix<Nrows,Ncols,T>::GetValuePtr (std::size_t inRow, std::size_t inCol) {
    return mData.data () + inRow + inCol * Nrows;
}

template<std::size_t Nrows, std::size_t Ncols, typename T>
MATH::Matrix<Nrows,Ncols,T>& MATH::Matrix<Nrows,Ncols,T>::operator= (const MATH::Matrix<Nrows,Ncols,T>& inMatrix) {
    std::copy (inMatrix.mData.begin (), inMatrix.mData.end (), mData.begin ()); 
    return *this;
}

template<std::size_t Nrows, std::size_t Ncols, typename T>
MATH::Matrix<Nrows,Ncols,T>& MATH::Matrix<Nrows,Ncols,T>::operator= (T inValue) {
    std::for_each (mData.begin (), mData.end (), [inValue] (auto& entry) {entry = inValue;}); 
    return *this;
}

template<std::size_t Nrows, std::size_t Ncols, typename T>
MATH::Matrix<Nrows,Ncols,T> MATH::Matrix<Nrows,Ncols,T>::operator+ (const MATH::Matrix<Nrows,Ncols,T>& inMatrix) const {
    Matrix mat;
    std::transform (mData.begin (), mData.end (), inMatrix.mData.begin (), mat.mData.begin (), std::plus<T> ()); 
    return mat;
}

template<std::size_t Nrows, std::size_t Ncols, typename T>
MATH::Matrix<Nrows,Ncols,T> MATH::Matrix<Nrows,Ncols,T>::operator+ (T inValue) const {
    Matrix mat;
    std::transform (mData.begin (), mData.end (), mat.mData.begin (), [inValue] (const auto& entry) {return entry + inValue;}); 
    return mat;
}

template<std::size_t Nrows, std::size_t Ncols, typename T>
MATH::Matrix<Nrows,Ncols,T> MATH::Matrix<Nrows,Ncols,T>::operator- (const MATH::Matrix<Nrows,Ncols,T>& inMatrix) const {
    Matrix mat;
    std::transform (mData.begin (), mData.end (), inMatrix.mData.begin (), mat.mData.begin (), std::minus<T> ()); 
    return mat;
}

template<std::size_t Nrows, std::size_t Ncols, typename T>
MATH::Matrix<Nrows,Ncols,T> MATH::Matrix<Nrows,Ncols,T>::operator- (T inValue) const {
    Matrix mat;
    std::transform (mData.begin (), mData.end (), mat.mData.begin (), [inValue] (const auto& entry) {return entry - inValue;}); 
    return mat;
}

template<std::size_t Nrows, std::size_t Ncols, typename T>
MATH::Matrix<Nrows,Ncols,T> MATH::Matrix<Nrows,Ncols,T>::operator* (T inValue) const {
    Matrix mat;
    std::transform (mData.begin (), mData.end (), mat.mData.begin (), [inValue] (const auto& entry) {return entry * inValue;}); 
    return mat;
}

template<std::size_t Nrows, std::size_t Ncols, typename T>
MATH::Matrix<Nrows,Ncols,T> MATH::Matrix<Nrows,Ncols,T>::operator/ (T inValue) const {
    Matrix mat;
    std::transform (mData.begin (), mData.end (), mat.mData.begin (), [inValue] (const auto& entry) {return entry / inValue;}); 
    return mat;
}

template<std::size_t Nrows, std::size_t Ncols, typename T>
MATH::Matrix<Ncols,Nrows,T> MATH::Matrix<Nrows,Ncols,T>::operator~ () const {
    Matrix<Ncols,Nrows,T> mat;
    for (std::size_t i (0u); i < Nrows; i++) {
        for (std::size_t j (0u); j < Ncols; j++) {
            mat(j,i) = (*this)(i,j);
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
