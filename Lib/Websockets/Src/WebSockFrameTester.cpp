
#include <algorithm>
#include "gtest/gtest.h"
#include "WebSockFrame.h"

TEST (WebSockFrameTester, Masking) {
    
    RFC6455::Frame frame;
    
    frame.mIsMasked = true;
    frame.mMask.assign (4u, 0xAB);
    frame.mPayload.assign (0x12, 0xAB);
    
    frame.Encode ();
    
    ASSERT_TRUE (std::all_of (frame.mPayload.begin(), frame.mPayload.end(), [](auto& val) { return val == 0x00; }));
    
    frame.Decode ();
    
    ASSERT_TRUE (std::all_of (frame.mPayload.begin(), frame.mPayload.end(), [](auto& val) { return val == 0xAB; }));
    
}
