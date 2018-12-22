
#ifndef _SIM_PORT_H_
#define _SIM_PORT_H_

#include <cstdint>
#include <string>

#include "SimPath.h"

namespace SIM {

class Port {

public:
    /**
     * Supported port types
     */
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

    /**
     * Info of the Port
     */
    struct Info {
        Path mPath;
        Type mType;
    };
    
    /**
     * Constructor
     */
    Port (const Path& inPath);
    
    /**
     * Destructor
     */
    virtual ~Port ();
    
    /**
     * Get the port name
     */
    const Path& GetPath () const;
    
    /**
     * Get the port type
     */
    virtual Type GetType () const = 0;
    
    /**
     * Get the value as string
     */
    virtual std::string GetStringValue () const = 0;
    
    /**
     * Set the value as string
     */
    virtual void SetStringValue (const std::string& inValue ) = 0;
    
    /**
     * Get the value as numeric
     */
    virtual double GetNumericValue () const = 0;
    
    /**
     * Set the value as numeric
     */
    virtual void SetNumericValue (double inValue) = 0;
    
    /**
     * Get port info
     */
    Info GetInfo () const;
    
protected:
    Path mPath;
};

}

#endif // _SIM_PORT_H_
