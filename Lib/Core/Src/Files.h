
#ifndef _FILES_H_
#define _FILES_H_

namespace OS {

class Buffer;

class Files {

public:
    Files () = default; 
    ~Files () = default;

public:
    static bool Read (const std::string& inPath, Buffer& outBuffer);
    static bool Write (const std::string& inPath, const Buffer& outBuffer);

};

}

#endif
