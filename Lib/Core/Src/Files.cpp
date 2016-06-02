
#include "Buffer.h"
#include "Files.h"

#include <iostream>
#include <fstream>

bool Core::Files::Read (const std::string& inPath, Core::Buffer& outBuffer) {

    std::ifstream stream;
    stream.open (inPath, std::ifstream::binary);

    if (stream.is_open ()) {

        stream.seekg (0, stream.end);
        outBuffer.Init (static_cast<unsigned> (stream.tellg ()));

        stream.seekg (0, stream.beg);
        stream.read (outBuffer.GetDataPointer (), outBuffer.GetSize ());
        stream.close ();

        return true;
    }

    return false;
}

bool Core::Files::Write (const std::string& inPath, const Core::Buffer& inBuffer) {

    std::ofstream stream;
    stream.open (inPath);

    if (stream.is_open ()) {

        stream.write (inBuffer.GetDataPointer (), inBuffer.GetSize ());
        stream.close ();

        return true;
    }

    return false;
}
