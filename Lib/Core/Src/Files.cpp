
#include "Files.h"

#include <iostream>
#include <fstream>

bool OS::Files::Read (const std::string& inPath, std::vector<uint8_t>& outBuffer) {

    std::ifstream stream;
    stream.open (inPath, std::ifstream::binary);

    if (stream.is_open ()) {

        stream.seekg (0, stream.end);
        outBuffer.resize (static_cast<std::size_t> (stream.tellg ()));

        stream.seekg (0, stream.beg);
        stream.read (reinterpret_cast<char*>(outBuffer.data ()), outBuffer.size ());
        stream.close ();

        return true;
    }

    return false;
}

bool OS::Files::Write (const std::string& inPath, const std::vector<uint8_t>& inBuffer) {

    std::ofstream stream;
    stream.open (inPath);

    if (stream.is_open ()) {

        stream.write (reinterpret_cast<const char*>(inBuffer.data ()), inBuffer.size ());
        stream.close ();

        return true;
    }

    return false;
}
