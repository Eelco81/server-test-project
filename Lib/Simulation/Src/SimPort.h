
#ifndef _SIM_PORT_H_
#define _SIM_PORT_H_

#include <cstdint>
#include <string>

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

public:
    /**
     * Constructor
     */
    Port (const std::string& inName);
    
    /**
     * Destructor
     */
    virtual ~Port ();
    
    /**
     * Get the port name
     */
    const std::string& GetName () const;
    
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
    
protected:
    std::string mName;
};

}

#endif // _SIM_PORT_H_
