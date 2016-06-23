
#include "gtest/gtest.h"

#include "Files.h"
#include "Buffer.h"

TEST (FilesTester, ReadFile) {

    std::string testFile = "../test/Files/readfile.txt";
    
    OS::Buffer buffer (0u);
    ASSERT_TRUE (OS::Files::Read (testFile, buffer));

    ASSERT_EQ (22u, buffer.GetSize ());
    ASSERT_EQ (22u, buffer.GetMaxSize ());

    std::string content;
    buffer.ToString (content);
    ASSERT_EQ (std::string ("This text must be read"), content);
}

TEST (FilesTester, WriteFile) {

    const std::string kTestFile ("../test/Files/tmp_writefile.txt");
    const std::string kTestData ("I must be written and read");
    const unsigned kTestSize (kTestData.length ());

    OS::Buffer writeBuffer (0u);
    OS::Buffer readBuffer (0u);

    writeBuffer.SetData (kTestData);
    ASSERT_TRUE (OS::Files::Write (kTestFile, writeBuffer));
    ASSERT_TRUE (OS::Files::Read (kTestFile, readBuffer));

    ASSERT_EQ (kTestSize, readBuffer.GetSize ());
    ASSERT_EQ (kTestSize, readBuffer.GetMaxSize ());

    std::string content;
    readBuffer.ToString (content);
    ASSERT_EQ (kTestData, content);
}