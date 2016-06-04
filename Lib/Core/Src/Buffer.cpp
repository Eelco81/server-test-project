
#include "Buffer.h"
#include <string.h>

OS::Buffer::Buffer (std::size_t inMaxSize) :
    mMaxSize (inMaxSize),
    mSize (0u)
{
    mBuffer = new char [mMaxSize];
}

OS::Buffer::~Buffer () {
    delete [] mBuffer;
}

void OS::Buffer::Init (std::size_t inSize) {
    ResizeContainer (inSize);
    ::memset (mBuffer, 0, inSize);
    mSize = inSize;
}

void OS::Buffer::Resize (std::size_t inSize) {
    ResizeContainer (inSize);
    mSize = inSize;
}

bool OS::Buffer::SetData (const std::string& inDataString) {
    return SetData (inDataString.c_str (), inDataString.length ());
}

bool OS::Buffer::SetData (const char* inBuffer, std::size_t inSize) {
    ResizeContainer (inSize);
    ::memcpy (mBuffer, inBuffer, inSize);
    mSize = inSize;
    return true;
}

void OS::Buffer::ToString (std::string& outDataString) const {
    outDataString.assign (mBuffer, mSize);
}

void OS::Buffer::ResizeContainer (std::size_t inSize) {
    if (inSize > mMaxSize) {
        mMaxSize = inSize;
        char* cache = mBuffer;
        mBuffer = new char [mMaxSize];
        ::memcpy (mBuffer, cache, mSize);
        delete [] cache;
    }
}