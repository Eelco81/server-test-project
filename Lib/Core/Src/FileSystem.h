
#ifndef _FILES_H_
#define _FILES_H_

#include <vector>
#include <cstdint>
#include <string>

namespace OS {

class FileSystem {

public:
    /**
     * Read a file
     */
    static bool Read (const std::string& inPath, std::vector<uint8_t>& outBuffer);
    static bool Read (const std::string& inPath, std::string& outBuffer);
    
    /**
     * Write a file
     */
    static bool Write (const std::string& inPath, const std::vector<uint8_t>& inBuffer);
    static bool Write (const std::string& inPath, const std::string& inBuffer);
    static bool Write (const std::string& inPath, const char* inBuffer, std::size_t inSize);
    
    /**
     * Check if a file exists
     */
    static bool Exists (const std::string& inPath);
    
    /**
     * Remove a file or directory
     */
    static bool Remove (const std::string& inPath);
    
    /**
     * Create directory
     */
    static bool CreateDir (const std::string& inPath);
    
    /**
     * List contents of a directory
     */
    static bool ListDir (const std::string& inPath, std::vector<std::string>& outFiles);
    
    /**
     * Get extension of a path
     */
    static std::string GetExtension (const std::string& inPath);

    /**
     * Get basename of a path
     */
    static std::string GetBasename (const std::string& inPath);

};

}

#endif
