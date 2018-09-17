
#ifndef _SIM_PORT_H_
#define _SIM_PORT_H_

#include <cstdint>
#include <string>

namespace SIM {

class Port {

public:
    enum Type : uint8_t {
        UNKNOWN = 0x00,
        BOOL,
        UINT8,
        UINT16,
        UINT32,
        UINT64,
        INT8,
        INT16,
        INT32,
        INT64,
        FLOAT,
        DOUBLE
    };

public:
    Port (const std::string& inName);
    virtual ~Port ();
    
    const std::string& GetName () const;
    virtual Type GetType () const = 0;
    virtual std::string GetStringValue () const = 0;
    
protected:
    std::string mName;
};

}

#endif // _SIM_PORT_H_
