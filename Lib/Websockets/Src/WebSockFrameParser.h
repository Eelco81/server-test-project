
#ifndef _RFC6455_FRAMEPARSER_H_
#define _RFC6455_FRAMEPARSER_H_

#include <vector>
#include <cstdint>

namespace RFC6455 {

class Frame;

class FrameParser {

public:

    /**
     * Destructor
     */
    virtual ~FrameParser ();

public:

    /**
     * Write buffer to the decoder
     */
    void Write (const std::vector<uint8_t>& inBuffer);

    /**
     * This method is called when a frame is decoded from 
     * the incoming buffer stream.
     */
    virtual void HandleFrame (const Frame& inFrame) = 0;

protected:
    std::vector<uint8_t> mBuffer;

};


} // end namespace RFC6455

#endif // _RFC6455_FRAMEPARSER_H_
