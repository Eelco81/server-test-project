
#ifndef _SIM_TIMER_H_
#define _SIM_TIMER_H_

#include <cstdint>

namespace SIM {

/**
 * The timer class provides the time stamping and ticking.
 */
class Timer {

public:
    /**
     * Constructor, timestep in microseconds, 
     */
    Timer (uint64_t mTimeStep);

public:
    /**
     * Reset the clock.
     */
    void Initialize ();
    
    /**
     * Tick the clock with one timestep.
     */
    void Tick ();
    
    /**
     * Get the current time as double in seconds.
     */
    double GetTime () const;

    /**
     * Get the current time step as double in seconds
     */
     double GetTimeStepValue() const;
     
    /**
     * Get the current time as tick in microseconds.
     */
    uint64_t GetTimeStamp () const;
    
    /**
     * Get the time step.
     */
    uint64_t GetTimeStep () const;
    
private:
    const uint64_t mTimeStep;
    uint64_t mTime;

};

} // namespace SIM

#endif // _SIM_TIMER_H_
