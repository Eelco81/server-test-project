
#ifndef _SIM_PATH_H_
#define _SIM_PATH_H_

#include <string>

namespace SIM {

struct Path {
    
    enum Type : uint8_t {
        INPUT = 0x00,
        OUTPUT = 0x01,
        PARAMETER = 0x02
    };
    
    Path (const std::string& inTarget);
    Path (const std::string& inBlockID, const std::string& inPortID, Type inType);

    std::string ToString () const;
    
    std::string mBlockID;
    std::string mPortID;
    Type mType;

};

}

#endif // _SIM_PATH_H_
