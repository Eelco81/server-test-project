
#ifndef _FILES_H_
#define _FILES_H_

#include <vector>
#include <cstdint>

namespace OS {

class Files {

public:
    Files () = default; 
    ~Files () = default;

public:
    static bool Read (const std::string& inPath, std::vector<uint8_t>& outBuffer);
    static bool Write (const std::string& inPath, const std::vector<uint8_t>& outBuffer);

};

}

#endif
