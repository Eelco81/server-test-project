
#ifndef _RF6455_FRAME_H_
#define _RF6455_FRAME_H_

#include <cstdint>
#include <vector>

namespace RFC6455 {

struct Frame {

public:
    Frame ();

/*
public:
    void SetFin (bool inFin);
    bool GetFin () const;

    void SetOpCode (uint8_t inOpCode);
    uint8_t GetOpCode () const;

    void SetIsMasked (bool inIsMasked);
    bool GetIsMasked () const;

    void SetPayload (uint8_t* inData, std::size_t inSize);
    const std::vector<uint8_t>& GetPayload () const;
    
    void SetMask (uint8_t* inData, std::size_t inSize);
    const std::vector<uint8_t>& GetMask () const;

private:
*/
    bool mFin;
    uint8_t mOpCode;
    bool mIsMasked;
    std::vector<uint8_t> mMask;
    std::vector<uint8_t> mPayload;

};

} // end namespace RFC6455

#endif // _RF6455_FRAME_H_
