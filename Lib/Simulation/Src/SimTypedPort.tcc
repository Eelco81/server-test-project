
#include <type_traits>
#include "SimException.h"

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

template<typename T>
std::string SIM::TypedPort<T>::GetStringValue () const {
    return std::to_string (*mValuePtr);
}

template<typename T>
void SIM::TypedPort<T>::SetStringValue (const std::string& inValue) {
    
    try {
        if (std::is_same<T,float>::value) {
            *mValuePtr = std::stof (inValue);
        }
        else if (std::is_same<T,double>::value) {
            *mValuePtr = std::stof (inValue);
        }
        else {
            *mValuePtr = std::stoi (inValue);
        }
    }
    catch (...) {
        throw Exception (std::string ("Invalid value string for port <") + GetName () + std::string (">"));
    }
}

template<typename T>
double SIM::TypedPort<T>::GetNumericValue () const {
    return static_cast<double> (*mValuePtr);
}

template<typename T>
void SIM::TypedPort<T>::SetNumericValue (double inValue) {
    *mValuePtr = static_cast<T> (inValue);
}
