
#ifndef _FILES_H_
#define _FILES_H_

#include <vector>
#include <cstdint>
#include <string>

namespace OS {

class Files {

public:
    static bool Read (const std::string& inPath, std::vector<uint8_t>& outBuffer);
    static bool Write (const std::string& inPath, const std::vector<uint8_t>& inBuffer);
    static std::string GetExtension (const std::string& inPath);

};

}

#endif
