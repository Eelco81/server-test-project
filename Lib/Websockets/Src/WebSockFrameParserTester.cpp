
#include "gtest/gtest.h"

#include "WebSockFrame.h"
#include "WebSockFrameParser.h"

namespace {

class TestFrameParser : public RFC6455::FrameParser {
public:
    void HandleFrame (const RFC6455::Frame& inFrame) override {
        mFrames.emplace_back (inFrame);
    }
    std::vector<RFC6455::Frame> mFrames;
};

} // end anonymous namespace

TEST (WebSockFrameParserTester, ShortMessageFormatNoPayload) {
    
    TestFrameParser parser;
    
    std::vector<uint8_t> raw = { 
        0b10001111, 0b00000000 
    };
    
    parser.Write (raw);
    
    ASSERT_EQ (1u, parser.mFrames.size ());
    ASSERT_TRUE (parser.mFrames[0u].mFin);
    ASSERT_EQ (0b1111, parser.mFrames[0u].mOpCode);
    ASSERT_FALSE (parser.mFrames[0u].mIsMasked);
    
    ASSERT_TRUE (parser.mFrames[0u].mMask.empty ());
    
    ASSERT_TRUE (parser.mFrames[0u].mPayload.empty ());
}

TEST (WebSockFrameParserTester, ShortMessageFormatWithPayload) {
    
    TestFrameParser parser;
    
    std::vector<uint8_t> raw = { 
        0b00001111, 0b00000010, 
        0x01, 0x02 
    };
    
    parser.Write (raw);
    
    ASSERT_EQ (1u, parser.mFrames.size ());
    ASSERT_FALSE (parser.mFrames[0u].mFin);
    ASSERT_EQ (0b1111, parser.mFrames[0u].mOpCode);
    ASSERT_FALSE (parser.mFrames[0u].mIsMasked);
    
    ASSERT_TRUE (parser.mFrames[0u].mMask.empty ());
    
    ASSERT_EQ (2u, parser.mFrames[0u].mPayload.size ());
    ASSERT_EQ (0x01, parser.mFrames[0u].mPayload[0u]);
    ASSERT_EQ (0x02, parser.mFrames[0u].mPayload[1u]);
}

TEST (WebSockFrameParserTester, ShortMessageFormatWithPayloadAndMask) {
    
    TestFrameParser parser;
    
    std::vector<uint8_t> raw = { 
        0b00001111, 0b10000010, 
        0xF1, 0xF2, 0xF3, 0xF4, 
        0x01, 0x02 
    };
    
    parser.Write (raw);
    
    ASSERT_EQ (1u, parser.mFrames.size ());
    ASSERT_FALSE (parser.mFrames[0u].mFin);
    ASSERT_EQ (0b1111, parser.mFrames[0u].mOpCode);
    ASSERT_TRUE (parser.mFrames[0u].mIsMasked);
    
    ASSERT_EQ (4u, parser.mFrames[0].mMask.size ());
    ASSERT_EQ (0xF1, parser.mFrames[0u].mMask[0u]);
    ASSERT_EQ (0xF2, parser.mFrames[0u].mMask[1u]);
    ASSERT_EQ (0xF3, parser.mFrames[0u].mMask[2u]);
    ASSERT_EQ (0xF4, parser.mFrames[0u].mMask[3u]);
    
    ASSERT_EQ (2u, parser.mFrames[0u].mPayload.size ());
    ASSERT_EQ (0x01, parser.mFrames[0u].mPayload[0u]);
    ASSERT_EQ (0x02, parser.mFrames[0].mPayload[1u]);
}

TEST (WebSockFrameParserTester, MediumMessageFormatWithPayload) {
    
    TestFrameParser parser;
    
    std::vector<uint8_t> raw = { 
        0b00001111, 0b01111110, 
        0x00, 0b00000010, 
        0x01, 0x02 
    };
    
    parser.Write (raw);
    
    ASSERT_EQ (1u, parser.mFrames.size ());
    ASSERT_FALSE (parser.mFrames[0u].mFin);
    ASSERT_EQ (0b1111, parser.mFrames[0u].mOpCode);
    ASSERT_FALSE (parser.mFrames[0u].mIsMasked);
    
    ASSERT_TRUE (parser.mFrames[0u].mMask.empty ());
    
    ASSERT_EQ (2u, parser.mFrames[0u].mPayload.size ());
    ASSERT_EQ (0x01, parser.mFrames[0u].mPayload[0u]);
    ASSERT_EQ (0x02, parser.mFrames[0u].mPayload[1u]);
}

TEST (WebSockFrameParserTester, MediumMessageFormatWithPayloadAndMask) {
    
    TestFrameParser parser;
    
    std::vector<uint8_t> raw = { 
        0b00001111, 0b11111110, 
        0x00, 0b00000010, 
        0xF1, 0xF2, 0xF3, 0xF4, 
        0x01, 0x02 
    };
    
    parser.Write (raw);
    
    ASSERT_EQ (1u, parser.mFrames.size ());
    ASSERT_FALSE (parser.mFrames[0u].mFin);
    ASSERT_EQ (0b1111, parser.mFrames[0u].mOpCode);
    ASSERT_TRUE (parser.mFrames[0u].mIsMasked);
    
    ASSERT_EQ (4u, parser.mFrames[0].mMask.size ());
    ASSERT_EQ (0xF1, parser.mFrames[0u].mMask[0u]);
    ASSERT_EQ (0xF2, parser.mFrames[0u].mMask[1u]);
    ASSERT_EQ (0xF3, parser.mFrames[0u].mMask[2u]);
    ASSERT_EQ (0xF4, parser.mFrames[0u].mMask[3u]);
    
    ASSERT_EQ (2u, parser.mFrames[0u].mPayload.size ());
    ASSERT_EQ (0x01, parser.mFrames[0u].mPayload[0u]);
    ASSERT_EQ (0x02, parser.mFrames[0].mPayload[1u]);
}


TEST (WebSockFrameParserTester, LongMessageFormatWithPayload) {
    
    TestFrameParser parser;
    
    std::vector<uint8_t> raw = { 
        0b00001111, 0b01111111, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0b00000010, 
        0x01, 0x02 
    };
    
    parser.Write (raw);
    
    ASSERT_EQ (1u, parser.mFrames.size ());
    ASSERT_FALSE (parser.mFrames[0u].mFin);
    ASSERT_EQ (0b1111, parser.mFrames[0u].mOpCode);
    ASSERT_FALSE (parser.mFrames[0u].mIsMasked);
    
    ASSERT_TRUE (parser.mFrames[0u].mMask.empty ());
    
    ASSERT_EQ (2u, parser.mFrames[0u].mPayload.size ());
    ASSERT_EQ (0x01, parser.mFrames[0u].mPayload[0u]);
    ASSERT_EQ (0x02, parser.mFrames[0u].mPayload[1u]);
}

TEST (WebSockFrameParserTester, LongMessageFormatWithPayloadAndMask) {
    
    TestFrameParser parser;
    
    std::vector<uint8_t> raw = { 
        0b00001111, 0b11111111, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0b00000010, 
        0xF1, 0xF2, 0xF3, 0xF4, 
        0x01, 0x02 
    };
    
    parser.Write (raw);
    
    ASSERT_EQ (1u, parser.mFrames.size ());
    ASSERT_FALSE (parser.mFrames[0u].mFin);
    ASSERT_EQ (0b1111, parser.mFrames[0u].mOpCode);
    ASSERT_TRUE (parser.mFrames[0u].mIsMasked);
    
    ASSERT_EQ (4u, parser.mFrames[0].mMask.size ());
    ASSERT_EQ (0xF1, parser.mFrames[0u].mMask[0u]);
    ASSERT_EQ (0xF2, parser.mFrames[0u].mMask[1u]);
    ASSERT_EQ (0xF3, parser.mFrames[0u].mMask[2u]);
    ASSERT_EQ (0xF4, parser.mFrames[0u].mMask[3u]);
    
    ASSERT_EQ (2u, parser.mFrames[0u].mPayload.size ());
    ASSERT_EQ (0x01, parser.mFrames[0u].mPayload[0u]);
    ASSERT_EQ (0x02, parser.mFrames[0].mPayload[1u]);
}
