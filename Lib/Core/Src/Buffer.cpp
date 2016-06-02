
#include "Buffer.h"
#include <string.h>

Core::Buffer::Buffer (std::size_t inMaxSize) :
    mMaxSize (inMaxSize),
    mSize (0u)
{
    mBuffer = new char [mMaxSize];
}

Core::Buffer::~Buffer () {
    delete [] mBuffer;
}

void Core::Buffer::Init (std::size_t inSize) {
    ResizeContainer (inSize);
    ::memset (mBuffer, 0, inSize);
    mSize = inSize;
}

void Core::Buffer::Resize (std::size_t inSize) {
    ResizeContainer (inSize);
    mSize = inSize;
}

bool Core::Buffer::SetData (const std::string& inDataString) {
    return SetData (inDataString.c_str (), inDataString.length ());
}

bool Core::Buffer::SetData (const char* inBuffer, std::size_t inSize) {
    ResizeContainer (inSize);
    ::memcpy (mBuffer, inBuffer, inSize);
    mSize = inSize;
    return true;
}

void Core::Buffer::ToString (std::string& outDataString) const {
    outDataString.assign (mBuffer, mSize);
}

void Core::Buffer::ResizeContainer (std::size_t inSize) {
    if (inSize > mMaxSize) {
        mMaxSize = inSize;
        char* cache = mBuffer;
        mBuffer = new char [mMaxSize];
        ::memcpy (mBuffer, cache, mSize);
        delete [] cache;
    }
}