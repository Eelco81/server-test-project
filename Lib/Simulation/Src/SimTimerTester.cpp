
#include "gtest/gtest.h"

#include "SimTimer.h"

TEST (SimTimerTester, TickTock) {

    SIM::Timer timer (100000u);
    
    ASSERT_EQ (0.0, timer.GetTime ());
    ASSERT_EQ (0u, timer.GetTimeStamp ());
    
    timer.Tick ();
    ASSERT_EQ (0.1, timer.GetTime ());
    ASSERT_EQ (100000u, timer.GetTimeStamp ());
    
    timer.Initialize ();
    ASSERT_EQ (0.0, timer.GetTime ());
    ASSERT_EQ (0u, timer.GetTimeStamp ());
    
    timer.Tick ();
    timer.Tick ();
    
    ASSERT_EQ (0.2, timer.GetTime ());
    ASSERT_EQ (200000u, timer.GetTimeStamp ());
    
    timer.Initialize ();
    
    ASSERT_EQ (0.0, timer.GetTime ());
    ASSERT_EQ (0u, timer.GetTimeStamp ());

}
