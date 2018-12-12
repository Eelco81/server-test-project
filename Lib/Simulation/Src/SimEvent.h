
#ifndef _SIM_EVENT_H_
#define _SIM_EVENT_H_

#include <string>

#include <json.hpp>
using json = nlohmann::json;

namespace SIM {

struct Event {
    
    /**
     * Constructor
     */
    Event (const std::string& inId);
    
    /**
     * Constructor
     */
    template <typename T>
    Event (const std::string& inId, const T& inData) :
        SIM::Event (inId)
    {
        mData = inData;
    }
    
    /**
     * Destructor
     */
    virtual ~Event ();
    
    /**
     * Convert to string.
     */
    std::string ToString () const;
    
    std::string mId;
    json mData;
};

} // end namespace SIM

#endif // _SIM_EVENT_H_
