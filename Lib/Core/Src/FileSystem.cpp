
#include "FileSystem.h"

#include <regex>
#include <iostream>
#include <fstream>
#include <experimental/filesystem>
namespace filesystem = std::experimental::filesystem;

bool OS::FileSystem::Read (const std::string& inPath, std::vector<uint8_t>& outBuffer) {
    std::ifstream stream;
    stream.open (inPath, std::ifstream::binary);
    if (stream.is_open ()) {
        stream.seekg (0, stream.end);
        outBuffer.resize (static_cast<std::size_t> (stream.tellg ()));
        stream.seekg (0, stream.beg);
        stream.read (reinterpret_cast<char*> (outBuffer.data ()), outBuffer.size ());
        stream.close ();
        return true;
    }
    return false;
}

bool OS::FileSystem::Read (const std::string& inPath, std::string& outBuffer) {
    std::ifstream stream;
    stream.open (inPath, std::ifstream::binary);
    if (stream.is_open ()) {
        stream.seekg (0, stream.end);
        outBuffer.resize (static_cast<std::size_t> (stream.tellg ()));
        stream.seekg (0, stream.beg);
        stream.read (outBuffer.data (), outBuffer.size ());
        stream.close ();
        return true;
    }
    return false;
}

bool OS::FileSystem::Write (const std::string& inPath, const std::vector<uint8_t>& inBuffer) {
    return Write (inPath, reinterpret_cast<const char*> (inBuffer.data ()), inBuffer.size ());
}

bool OS::FileSystem::Write (const std::string& inPath, const std::string& inBuffer) {
    return Write (inPath, inBuffer.data (), inBuffer.size ());
}

bool OS::FileSystem::Write (const std::string& inPath, const char* inBuffer, std::size_t inSize) {
    std::ofstream stream;
    stream.open (inPath);
    if (stream.is_open ()) {
        stream.write (inBuffer, inSize);
        stream.close ();
        return true;
    }
    return false;
}

bool OS::FileSystem::Exists (const std::string& inPath) {
    return filesystem::exists (filesystem::path (inPath)); 
}

bool OS::FileSystem::Remove (const std::string& inPath) {
    return filesystem::remove_all (filesystem::path (inPath)); 
}

bool OS::FileSystem::CreateDir (const std::string& inPath) {
    return filesystem::create_directory (filesystem::path (inPath)); 
}

bool OS::FileSystem::ListDir (const std::string& inPath, std::vector<std::string>& outFiles) {
    if (!Exists (inPath)) {
        return false;
    }
    outFiles.clear ();
    for (const auto& p : filesystem::directory_iterator (inPath)) {
        outFiles.push_back (p.path().filename().string ());
    }
    return true;
}

std::string OS::FileSystem::GetExtension (const std::string& inPath) {
    return filesystem::path (inPath).extension ();
}

std::string OS::FileSystem::GetBasename (const std::string& inPath) {
    return filesystem::path (inPath).stem ();
}
