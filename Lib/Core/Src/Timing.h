
#ifndef _OS_TIMING_H_
#define _OS_TIMING_H_

#include <cstddef>
#include <cstdint>
#include <string>

namespace OS {

namespace Timing {
    
    /**
     * Get the current system time in milliseconds
     */
    uint64_t Now ();
    
    /**
     * Convert a timestamp to string
     */
    std::string ToString (uint64_t inTime);
    
    /**
     * Convert a timestamp to http compatible string
     */
    std::string ToStdString (uint64_t inTime);
    
    /**
     * Sleep for a given ammount of milliseconds
     */
    void Sleep (uint64_t inMilliSeconds);
    
} // end namespace Timing

} // end namespace OS

#endif // _OS_TIMING_H_
