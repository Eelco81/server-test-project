
#ifndef _SIM_TYPEDPORT_H_
#define _SIM_TYPEDPORT_H_

#include "SimPort.h"

namespace SIM {

template <typename T>
class TypedPort : public Port {

public:
    TypedPort (const Path& inPath, T* inValue);
    TypedPort (const Path& inPath);
    virtual ~TypedPort ();
    
    TypedPort& operator= (const TypedPort& inMatrix);
    TypedPort& operator= (T inValue);

    T Get () const;
    void Set (T inValue);

public: 
    // Port Interface
    virtual Type GetType () const override;
    virtual std::string GetStringValue () const override;
    virtual void SetStringValue (const std::string& inValue) override;
    virtual double GetNumericValue () const override;
    virtual void SetNumericValue (double inValue) override;
    
protected:
    T* mValuePtr;
    bool mHasOwnership;
};

}

#include "SimTypedPort.tcc"

#endif // _SIM_TYPEDPORT_H_
