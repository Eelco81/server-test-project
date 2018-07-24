
#include "gtest/gtest.h"

#include "Files.h"
#include "Buffer.h"

TEST (FilesTester, ReadFile) {

    std::string testFile = "../Test/Files/readfile.txt";
    
    std::vector<uint8_t> buffer (0u);
    ASSERT_TRUE (OS::Files::Read (testFile, buffer));

    ASSERT_EQ (22u, buffer.size ());
    
    std::string content (reinterpret_cast<char*> (buffer.data ()));
    
    ASSERT_EQ (std::string ("This text must be read"), content);
}

TEST (FilesTester, WriteFile) {

    const std::string kTestFile ("../Test/Files/tmp_writefile.txt");

    const std::vector<uint8_t> writeBuffer (100000u, 0xFF);
    std::vector<uint8_t> readBuffer (0u);

    ASSERT_TRUE (OS::Files::Write (kTestFile, writeBuffer));
    ASSERT_TRUE (OS::Files::Read (kTestFile, readBuffer));

    ASSERT_EQ (writeBuffer.size(), readBuffer.size ());
    
    ASSERT_TRUE (std::equal (writeBuffer.begin (), writeBuffer.end (), readBuffer.begin ()));
}