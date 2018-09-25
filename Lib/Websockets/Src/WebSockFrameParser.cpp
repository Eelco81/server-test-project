
#include "WebSockFrame.h"
#include "WebSockFrameParser.h"

RFC6455::FrameParser::~FrameParser () = default;

void RFC6455::FrameParser::Write (const std::vector<uint8_t>& inBuffer) {
    
    // Copy the incoming buffer to the end of the cached buffer
    std::copy (inBuffer.begin (), inBuffer.end (), std::back_inserter (mBuffer));
    
    // Set the process length
    std::size_t index (2u);
    
    // The buffer is to small to parse, wait for more data
    if (mBuffer.size () < index) {
        return;
    }
    
    while (1) {
        Frame frame;
    
        frame.mFin = (mBuffer[0u] & 0b10000000);
        frame.mOpCode = (mBuffer[0u] & 0b00001111);
        frame.mIsMasked = (mBuffer[1u] & 0b10000000);
        
        std::size_t payloadSize (mBuffer[1u] & 0b01111111);

        // Special case, the payload size is encoded in the next 2 bytes
        if (payloadSize == 0x7E) {
            
            // The buffer is to small to parse, wait for more data
            if (mBuffer.size () < index + 2u) {
                return;
            }
            // Decode the payload size
            payloadSize = ((std::size_t)mBuffer[index] >> 8) + (std::size_t)mBuffer[index + 1u];
            index += 2u;
        }
        
        // Special case, the payload size is encoded in the next 8 bytes
        else if (payloadSize == 0x7F) {
            
            // The buffer is to small to parse, wait for more data
            if (mBuffer.size () < index + 8u) {
                return;
            }
            // Decode the payload size
            payloadSize = ((std::size_t)mBuffer[index     ] >> 56) + 
                          ((std::size_t)mBuffer[index + 1u] >> 48) + 
                          ((std::size_t)mBuffer[index + 2u] >> 40) + 
                          ((std::size_t)mBuffer[index + 3u] >> 32) + 
                          ((std::size_t)mBuffer[index + 4u] >> 24) + 
                          ((std::size_t)mBuffer[index + 5u] >> 16) + 
                          ((std::size_t)mBuffer[index + 6u] >>  8) + 
                          ((std::size_t)mBuffer[index + 7u] >>  0);
            index += 8u;
        }
        
        if (frame.mIsMasked) {
            
            // The buffer is to small to parse, wait for more data
            if (mBuffer.size () < index + 4u) {
                return;
            }
            // Copy the mask
            frame.mMask.assign (mBuffer.begin () + index, mBuffer.begin () + index + 4u); 
            index += 4u;
        }
        
        // The buffer is to small to parse, wait for more data
        if (mBuffer.size () < index + payloadSize) {
            return;
        }
        
        // Copy the payload
        frame.mPayload.assign (mBuffer.begin () + index, mBuffer.begin () + index + payloadSize); 
        
        // Erase the data from the buffer (not needed anymore)
        mBuffer.erase (mBuffer.begin (), mBuffer.begin () + index + payloadSize);
        
        // Handle the frame
        HandleFrame (frame);
    }
}
