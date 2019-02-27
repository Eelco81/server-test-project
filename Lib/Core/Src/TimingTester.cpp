
#include "gtest/gtest.h"
#include "Timing.h"

class TimingTester : public ::testing::TestWithParam<uint64_t> {};

INSTANTIATE_TEST_CASE_P (TimingTester, TimingTester,
    ::testing::Values ( 1000u, 2000u, 3000u, 4000u, 5000u, 
                        10000u, 20000u, 50000u, 100000u, 1000000u )
);

TEST_P (TimingTester, Sleep) {
    
    const auto t1 (OS::Timing::Now ());
    OS::Timing::Sleep (GetParam ());
    const auto t2 (OS::Timing::Now ());
    ASSERT_GT (t2, t1);
    ASSERT_NEAR (t2 - t1, GetParam (), 1500u);
    
}
