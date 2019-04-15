
#include "gtest/gtest.h"

#include "WebSockFrame.h"
#include "WebSockFrameDecoder.h"

namespace {

class TestFrameDecoder : public RFC6455::FrameDecoder {
public:
    TestFrameDecoder () :
        RFC6455::FrameDecoder () 
    {
        Pipe (this, &TestFrameDecoder::HandleFrame);
    }
    void HandleFrame (const RFC6455::Frame& inFrame) {
        mFrames.emplace_back (inFrame);
    }
    std::vector<RFC6455::Frame> mFrames;
};

} // end anonymous namespace

TEST (WebSockFrameDecoderTester, ShortMessageFormatNoPayload) {
    
    TestFrameDecoder decoder;
    
    const std::vector<uint8_t> raw = { 
        0b10001111, 0b00000000 
    };
    
    decoder.Write (raw);
    
    ASSERT_EQ (1u, decoder.mFrames.size ());
    ASSERT_TRUE (decoder.mFrames[0u].mFin);
    ASSERT_EQ (0b1111, decoder.mFrames[0u].mOpCode);
    ASSERT_FALSE (decoder.mFrames[0u].mIsMasked);
    
    ASSERT_TRUE (decoder.mFrames[0u].mMask.empty ());
    
    ASSERT_TRUE (decoder.mFrames[0u].mPayload.empty ());
}

TEST (WebSockFrameDecoderTester, ShortMessageFormatWithPayload) {
    
    TestFrameDecoder decoder;
    
    std::vector<uint8_t> raw = { 
        0b00001111, 0b00000010, 
        0x01, 0x02 
    };
    
    decoder.Write (raw);
    
    ASSERT_EQ (1u, decoder.mFrames.size ());
    ASSERT_FALSE (decoder.mFrames[0u].mFin);
    ASSERT_EQ (0b1111, decoder.mFrames[0u].mOpCode);
    ASSERT_FALSE (decoder.mFrames[0u].mIsMasked);
    
    ASSERT_TRUE (decoder.mFrames[0u].mMask.empty ());
    
    ASSERT_EQ (2u, decoder.mFrames[0u].mPayload.size ());
    ASSERT_EQ (0x01, decoder.mFrames[0u].mPayload[0u]);
    ASSERT_EQ (0x02, decoder.mFrames[0u].mPayload[1u]);
}

TEST (WebSockFrameDecoderTester, ShortMessageFormatWithPayloadAndMask) {
    
    TestFrameDecoder decoder;
    
    std::vector<uint8_t> raw = { 
        0b00001111, 0b10000010, 
        0xF1, 0xF2, 0xF3, 0xF4, 
        0x01, 0x02 
    };
    
    decoder.Write (raw);
    
    ASSERT_EQ (1u, decoder.mFrames.size ());
    ASSERT_FALSE (decoder.mFrames[0u].mFin);
    ASSERT_EQ (0b1111, decoder.mFrames[0u].mOpCode);
    ASSERT_TRUE (decoder.mFrames[0u].mIsMasked);
    
    ASSERT_EQ (4u, decoder.mFrames[0].mMask.size ());
    ASSERT_EQ (0xF1, decoder.mFrames[0u].mMask[0u]);
    ASSERT_EQ (0xF2, decoder.mFrames[0u].mMask[1u]);
    ASSERT_EQ (0xF3, decoder.mFrames[0u].mMask[2u]);
    ASSERT_EQ (0xF4, decoder.mFrames[0u].mMask[3u]);
    
    ASSERT_EQ (2u, decoder.mFrames[0u].mPayload.size ());
    ASSERT_EQ (0x01, decoder.mFrames[0u].mPayload[0u]);
    ASSERT_EQ (0x02, decoder.mFrames[0].mPayload[1u]);
}

TEST (WebSockFrameDecoderTester, MediumMessageFormatWithPayload) {
    
    TestFrameDecoder decoder;
    
    std::vector<uint8_t> raw = { 
        0b00001111, 0b01111110, 
        0x00, 0b00000010, 
        0x01, 0x02 
    };
    
    decoder.Write (raw);
    
    ASSERT_EQ (1u, decoder.mFrames.size ());
    ASSERT_FALSE (decoder.mFrames[0u].mFin);
    ASSERT_EQ (0b1111, decoder.mFrames[0u].mOpCode);
    ASSERT_FALSE (decoder.mFrames[0u].mIsMasked);
    
    ASSERT_TRUE (decoder.mFrames[0u].mMask.empty ());
    
    ASSERT_EQ (2u, decoder.mFrames[0u].mPayload.size ());
    ASSERT_EQ (0x01, decoder.mFrames[0u].mPayload[0u]);
    ASSERT_EQ (0x02, decoder.mFrames[0u].mPayload[1u]);
}

TEST (WebSockFrameDecoderTester, MediumMessageFormatWithPayloadAndMask) {
    
    TestFrameDecoder decoder;
    
    std::vector<uint8_t> raw = { 
        0b00001111, 0b11111110, 
        0x00, 0b00000010, 
        0xF1, 0xF2, 0xF3, 0xF4, 
        0x01, 0x02 
    };
    
    decoder.Write (raw);
    
    ASSERT_EQ (1u, decoder.mFrames.size ());
    ASSERT_FALSE (decoder.mFrames[0u].mFin);
    ASSERT_EQ (0b1111, decoder.mFrames[0u].mOpCode);
    ASSERT_TRUE (decoder.mFrames[0u].mIsMasked);
    
    ASSERT_EQ (4u, decoder.mFrames[0].mMask.size ());
    ASSERT_EQ (0xF1, decoder.mFrames[0u].mMask[0u]);
    ASSERT_EQ (0xF2, decoder.mFrames[0u].mMask[1u]);
    ASSERT_EQ (0xF3, decoder.mFrames[0u].mMask[2u]);
    ASSERT_EQ (0xF4, decoder.mFrames[0u].mMask[3u]);
    
    ASSERT_EQ (2u, decoder.mFrames[0u].mPayload.size ());
    ASSERT_EQ (0x01, decoder.mFrames[0u].mPayload[0u]);
    ASSERT_EQ (0x02, decoder.mFrames[0].mPayload[1u]);
}


TEST (WebSockFrameDecoderTester, LongMessageFormatWithPayload) {
    
    TestFrameDecoder decoder;
    
    std::vector<uint8_t> raw = { 
        0b00001111, 0b01111111, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0b00000010, 
        0x01, 0x02 
    };
    
    decoder.Write (raw);
    
    ASSERT_EQ (1u, decoder.mFrames.size ());
    ASSERT_FALSE (decoder.mFrames[0u].mFin);
    ASSERT_EQ (0b1111, decoder.mFrames[0u].mOpCode);
    ASSERT_FALSE (decoder.mFrames[0u].mIsMasked);
    
    ASSERT_TRUE (decoder.mFrames[0u].mMask.empty ());
    
    ASSERT_EQ (2u, decoder.mFrames[0u].mPayload.size ());
    ASSERT_EQ (0x01, decoder.mFrames[0u].mPayload[0u]);
    ASSERT_EQ (0x02, decoder.mFrames[0u].mPayload[1u]);
}

TEST (WebSockFrameDecoderTester, LongMessageFormatWithPayloadAndMask) {
    
    TestFrameDecoder decoder;
    
    std::vector<uint8_t> raw = { 
        0b00001111, 0b11111111, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0b00000010, 
        0xF1, 0xF2, 0xF3, 0xF4, 
        0x01, 0x02 
    };
    
    decoder.Write (raw);
    
    ASSERT_EQ (1u, decoder.mFrames.size ());
    ASSERT_FALSE (decoder.mFrames[0u].mFin);
    ASSERT_EQ (0b1111, decoder.mFrames[0u].mOpCode);
    ASSERT_TRUE (decoder.mFrames[0u].mIsMasked);
    
    ASSERT_EQ (4u, decoder.mFrames[0].mMask.size ());
    ASSERT_EQ (0xF1, decoder.mFrames[0u].mMask[0u]);
    ASSERT_EQ (0xF2, decoder.mFrames[0u].mMask[1u]);
    ASSERT_EQ (0xF3, decoder.mFrames[0u].mMask[2u]);
    ASSERT_EQ (0xF4, decoder.mFrames[0u].mMask[3u]);
    
    ASSERT_EQ (2u, decoder.mFrames[0u].mPayload.size ());
    ASSERT_EQ (0x01, decoder.mFrames[0u].mPayload[0u]);
    ASSERT_EQ (0x02, decoder.mFrames[0].mPayload[1u]);
}
