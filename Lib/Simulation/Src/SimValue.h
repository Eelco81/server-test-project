
#ifndef _SIM_VALUE_H_
#define _SIM_VALUE_H_

#include <cstdint>

#include "SimPath.h"

namespace SIM {

struct Value {
    
    Value () :
        mPath (),
        mValue (0.0)
    {}
    
    Value (const Path& inPath, double inValue) :
        mPath (inPath),
        mValue(inValue)
    {}
    
    Path mPath;
    double mValue;

};

}

#endif // _SIM_VALUE_H_
