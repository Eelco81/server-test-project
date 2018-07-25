

#include "gtest/gtest.h"
#include "Timing.h"


TEST (Timing, Sleep) {
    
    const uint64_t kSleepTime (100u);
    
    const auto t1 (OS::Timing::Now());
    
    OS::Timing::Sleep (kSleepTime);
    
    const auto t2 (OS::Timing::Now());

    ASSERT_NEAR (t1 + kSleepTime, t2, 20u);
}
