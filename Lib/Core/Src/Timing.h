
#ifndef _OS_TIMING_H_
#define _OS_TIMING_H_

#include <cstddef>
#include <cstdint>
#include <string>

namespace OS {

namespace Timing {
    
    /**
     * Setup the clock
     */
    void Initialize ();
    
    /**
     * Get the current system time in microseconds
     */
    uint64_t Now ();
    
    /**
     * Convert a timestamp in microseconds to string
     */
    std::string ToString (uint64_t inTime);
    
    /**
     * Convert a timestamp in microseconds to http compatible string
     */
    std::string ToStdString (uint64_t inTime);
    
    /**
     * Sleep for a given ammount of microseconds
     */
    void Sleep (uint64_t inSleep);
    
} // end namespace Timing

} // end namespace OS

#endif // _OS_TIMING_H_
