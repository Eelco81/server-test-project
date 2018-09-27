
#include <algorithm>
#include "gtest/gtest.h"
#include "WebSockFrame.h"

TEST (WebSockFrameTester, ToBufferFirstByte) {
    
    RFC6455::Frame frame;
    {
        frame.mFin = true;
        frame.mOpCode = RFC6455::Frame::PING;
        const auto buffer (frame.ToBuffer ());
        ASSERT_EQ (0b10001001, buffer[0]);
    }
    {
        frame.mFin = false;
        frame.mOpCode = RFC6455::Frame::TEXT;
        const auto buffer (frame.ToBuffer ());
        ASSERT_EQ (0b00000001, buffer[0]);
    }
}

TEST (WebSockFrameTester, ToBufferPayload) {
    
    RFC6455::Frame frame;
    {
        frame.mPayload.assign (0x12, 0xFF);
        const auto buffer (frame.ToBuffer ());
        ASSERT_EQ (0x12 + 2u, buffer.size ());
        ASSERT_EQ (0x12, buffer[1]);
        ASSERT_TRUE (std::all_of (buffer.begin() + 2u, buffer.end(), [](auto& val) { return val == 0xFF; }));
    }
    {
        frame.mPayload.assign (0x7D, 0xFF);
        const auto buffer (frame.ToBuffer ());
        ASSERT_EQ (0x7D + 2u, buffer.size ());
        ASSERT_EQ (0x7D, buffer[1]);
        ASSERT_TRUE (std::all_of (buffer.begin() + 2u, buffer.end(), [](auto& val) { return val == 0xFF; }));
    }
    {
        frame.mPayload.assign (0xEB90, 0xFF);
        const auto buffer (frame.ToBuffer ());
        ASSERT_EQ (0xEB90 + 4u, buffer.size ());
        ASSERT_EQ (0x7E, buffer[1]);
        ASSERT_EQ (0xEB, buffer[2]);
        ASSERT_EQ (0x90, buffer[3]);
        ASSERT_TRUE (std::all_of (buffer.begin() + 4u, buffer.end(), [](auto& val) { return val == 0xFF; }));
    }
    {
        frame.mPayload.resize (0xABCDEF);
        frame.mPayload.assign (0xABCDEF, 0xFF);
        const auto buffer (frame.ToBuffer ());
        ASSERT_EQ (0xABCDEF + 10u, buffer.size ());
        ASSERT_EQ (0x7F, buffer[1]);
        ASSERT_EQ (0x00, buffer[2]);
        ASSERT_EQ (0x00, buffer[3]);
        ASSERT_EQ (0x00, buffer[4]);
        ASSERT_EQ (0x00, buffer[5]);
        ASSERT_EQ (0x00, buffer[6]);
        ASSERT_EQ (0xAB, buffer[7]);
        ASSERT_EQ (0xCD, buffer[8]);
        ASSERT_EQ (0xEF, buffer[9]);
        ASSERT_TRUE (std::all_of (buffer.begin() + 10u, buffer.end(), [](auto& val) { return val == 0xFF; }));
    }
}

TEST (WebSockFrameTester, ToBufferMask) {
    
    RFC6455::Frame frame;
    
    frame.mIsMasked = true;
    frame.mMask.assign (4u, 0xAB);
    frame.mPayload.assign (0x12, 0xFF);
    
    const auto buffer (frame.ToBuffer ());
    ASSERT_EQ (0x12 + 2u + 4u, buffer.size ());
    ASSERT_EQ (0x12 + 0b10000000, buffer[1]);
    ASSERT_TRUE (std::all_of (buffer.begin() + 2u, buffer.begin() + 6u, [](auto& val) { return val == 0xAB; }));
    ASSERT_TRUE (std::all_of (buffer.begin() + 6u, buffer.end(), [](auto& val) { return val == 0xFF; }));
    
}

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
