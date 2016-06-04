
#ifndef _Buffer_H_
#define _Buffer_H_

#include <string>

namespace OS {

class Buffer {

public:
    Buffer () = delete;
    Buffer (const Buffer& inBuffer) = delete;

    Buffer (std::size_t inMaxLength);
    ~Buffer ();

public:
    void Init (std::size_t inSize);
    void Resize (std::size_t inSize);

    inline std::size_t GetSize () const { return mSize; }
    inline std::size_t GetMaxSize () const { return mMaxSize; }
    inline char* GetDataPointer () { return mBuffer; }
    inline const char* GetDataPointer () const { return mBuffer; }

    bool SetData (const std::string& inDataString);
    bool SetData (const char* inData, std::size_t inBufferSize);

    void ToString (std::string& outDataString) const;

private:
    void ResizeContainer (std::size_t inSize);

private:
    std::size_t mMaxSize;
    std::size_t mSize;
    char* mBuffer;
};

}



#endif
