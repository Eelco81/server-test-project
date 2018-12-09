
template<std::size_t N, std::size_t M, std::size_t K, typename T>
MATH::DynSys<N,M,K,T>::DynSys () :
    mA ((T)0),
    mB ((T)0),
    mC ((T)0),
    mD ((T)0),
    mX ((T)0),
    mY ((T)0),
    mDT ((T)0)
{
}

template<std::size_t N, std::size_t M, std::size_t K, typename T>
MATH::DynSys<N,M,K,T>::DynSys (const MATH::Matrix<N,N,T>& inA, const MATH::Matrix<N,M,T>& inB, const MATH::Matrix<K,N,T>& inC, const MATH::Matrix<K,M,T>& inD, T inDT, const MATH::Vector<N,T>& inX) :
    mA (inA),
    mB (inB),
    mC (inC),
    mD (inD),
    mX (inX),
    mY ((T)0),
    mDT (inDT)
{
}

template<std::size_t N, std::size_t M, std::size_t K, typename T>
void MATH::DynSys<N,M,K,T>::Reset (const MATH::Matrix<N,N,T>& inA, const MATH::Matrix<N,M,T>& inB, const MATH::Matrix<K,N,T>& inC, const MATH::Matrix<K,M,T>& inD, T inDT, const MATH::Vector<N,T>& inX) {
    mA = inA;
    mB = inB;
    mC = inC;
    mD = inD;
    mX = inX;
    mY = (T)0;
    mDT = inDT;
}

template<std::size_t N, std::size_t M, std::size_t K, typename T>
MATH::DynSys<N,M,K,T>::~DynSys () = default;

template<std::size_t N, std::size_t M, std::size_t K, typename T>
void MATH::DynSys<N,M,K,T>::Update (const MATH::Vector<M,T>& inU) {
    mX = mX + ( mA * mX + mB * inU ) * mDT;
    mY = mC * mX + mD * inU;
}



