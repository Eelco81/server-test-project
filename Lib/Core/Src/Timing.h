
#ifndef _OS_TIMING_H_
#define _OS_TIMING_H_

#include <cstddef>
#include <cstdint>
#include <string>

namespace OS {
    
namespace Timing {
    
    uint64_t Now ();
    
    std::string ToString (uint64_t inTime);
    
    void Sleep (uint64_t inMilliSeconds);
    
} // end namespace Timing

} // end namespace OS

#endif // _OS_TIMING_H_
