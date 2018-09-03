
#include <type_traits>

template <typename T>
SIM::TypedPort<T>::TypedPort (const std::string& inName, T* inValuePtr) : 
    Port (inName),
    mValuePtr (inValuePtr),
    mHasOwnership (false)
{
}

template <typename T>
SIM::TypedPort<T>::TypedPort (const std::string& inName) : 
    Port (inName),
    mValuePtr (new T (0)),
    mHasOwnership (true)
{
}

template <typename T>
SIM::TypedPort<T>::~TypedPort () {
    if (mHasOwnership) {
        delete mValuePtr;
    }
} 

template <typename T>
SIM::TypedPort<T>& SIM::TypedPort<T>::operator= (const TypedPort<T>& inPort) {
    *mValuePtr = *inPort.mValuePtr;
    return *this;
}

template <typename T>
SIM::TypedPort<T>& SIM::TypedPort<T>::operator= (T inValue) {
    *mValuePtr = inValue;
    return *this;
}

template <typename T>
T SIM::TypedPort<T>::Get () const { 
    return *mValuePtr; 
}

template <typename T>
void SIM::TypedPort<T>::Set (T inValue) { 
    *mValuePtr = inValue; 
}

template<typename T>
SIM::Port::Type SIM::TypedPort<T>::GetType () const {
    if (std::is_same<T,bool>::value) {
        return Port::Type::BOOL;
    }
    if (std::is_same<T,uint8_t>::value) {
        return Port::Type::UINT8;
    }
    if (std::is_same<T,uint16_t>::value) {
        return Port::Type::UINT16;
    }
    if (std::is_same<T,uint32_t>::value) {
        return Port::Type::UINT32;
    }
    if (std::is_same<T,uint64_t>::value) {
        return Port::Type::UINT64;
    }
    if (std::is_same<T,int8_t>::value) {
        return Port::Type::INT8;
    }
    if (std::is_same<T,int16_t>::value) {
        return Port::Type::INT16;
    }
    if (std::is_same<T,int32_t>::value) {
        return Port::Type::INT32;
    }
    if (std::is_same<T,int64_t>::value) {
        return Port::Type::INT64;
    }
    if (std::is_same<T,float>::value) {
        return Port::Type::FLOAT;
    }
    if (std::is_same<T,double>::value) {
        return Port::Type::DOUBLE;
    }
    return Port::Type::UNKNOWN;
}

/* 
template<>
SIM::Port::Type SIM::TypedPort<bool>::GetType () const { 
    return Port::Type::BOOL; 
}

template<>
SIM::Port::Type SIM::TypedPort<uint8_t>::GetType () const {
    return Port::Type::UINT8;
}

template<>
SIM::Port::Type SIM::TypedPort<uint16_t>::GetType () const {
    return Port::Type::UINT16;
}

template<>
SIM::Port::Type SIM::TypedPort<uint32_t>::GetType () const {
    return Port::Type::UINT32;
}

template<>
SIM::Port::Type SIM::TypedPort<uint64_t>::GetType () const {
    return Port::Type::UINT64;
}

template<>
SIM::Port::Type SIM::TypedPort<int8_t>::GetType () const {
    return Port::Type::INT8;
}

template<>
SIM::Port::Type SIM::TypedPort<int16_t>::GetType () const {
    return Port::Type::INT16;
}

template<>
SIM::Port::Type SIM::TypedPort<int32_t>::GetType () const {
    return Port::Type::INT32;
}

template<>
SIM::Port::Type SIM::TypedPort<int64_t>::GetType () const {
    return Port::Type::INT64;
}

template<>
SIM::Port::Type SIM::TypedPort<float>::GetType () const {
    return Port::Type::FLOAT;
}

template<>
SIM::Port::Type SIM::TypedPort<double>::GetType () const {
    return Port::Type::DOUBLE;
}
*/
