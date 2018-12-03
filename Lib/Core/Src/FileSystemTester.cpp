
#include "gtest/gtest.h"

#include "FileSystem.h"

TEST (FileSystemTester, ReadFile) {

    std::string testFile = "../Test/Files/readfile.txt";
    
    std::vector<uint8_t> buffer (0u);
    ASSERT_TRUE (OS::FileSystem::Read (testFile, buffer));

    ASSERT_EQ (22u, buffer.size ());
    
    std::string content (reinterpret_cast<char*> (buffer.data ()), buffer.size ());
    
    ASSERT_EQ (std::string ("This text must be read"), content);
}

TEST (FileSystemTester, WriteFile) {

    const std::string kTestFile ("../Test/Files/tmp_writefile.txt");

    const std::vector<uint8_t> writeBuffer (100000u, 0xFF);
    std::vector<uint8_t> readBuffer (0u);

    ASSERT_TRUE (OS::FileSystem::Write (kTestFile, writeBuffer));
    ASSERT_TRUE (OS::FileSystem::Read (kTestFile, readBuffer));

    ASSERT_EQ (writeBuffer.size (), readBuffer.size ());
    
    ASSERT_TRUE (std::equal (writeBuffer.begin (), writeBuffer.end (), readBuffer.begin ()));

    ASSERT_TRUE (OS::FileSystem::Remove (kTestFile));
    ASSERT_FALSE (OS::FileSystem::Remove (kTestFile));
}

TEST (FileSystemTester, WriteFileString) {

    const std::string kTestFile ("../Test/Files/tmp_writefile.txt");

    const std::string writeBuffer ("very special data");
    std::string readBuffer;

    ASSERT_TRUE (OS::FileSystem::Write (kTestFile, writeBuffer));
    ASSERT_TRUE (OS::FileSystem::Read (kTestFile, readBuffer));

    ASSERT_EQ (writeBuffer.size (), readBuffer.size ());
    
    ASSERT_EQ (writeBuffer, readBuffer);

    ASSERT_TRUE (OS::FileSystem::Remove (kTestFile));
    ASSERT_FALSE (OS::FileSystem::Remove (kTestFile));
}

TEST (FileSystemTester, FileExists) {

    ASSERT_TRUE (OS::FileSystem::Exists ("../Test/Files/readfile.txt"));
    ASSERT_TRUE (OS::FileSystem::Exists ("../Test/Files"));
    ASSERT_FALSE (OS::FileSystem::Exists ("nonexistingfile"));
}

TEST (FileSystemTester, CreateDirectory) {
    
    const std::string kTestDir ("../Test/Files/tmp");
    ASSERT_FALSE (OS::FileSystem::Exists (kTestDir));
    ASSERT_TRUE (OS::FileSystem::CreateDir (kTestDir));
    ASSERT_TRUE (OS::FileSystem::Exists (kTestDir));
    ASSERT_TRUE (OS::FileSystem::Remove (kTestDir));
    ASSERT_FALSE (OS::FileSystem::Exists (kTestDir));
}

TEST (FileSystemTester, ListDirectory) {
    
    const std::vector<uint8_t> writeBuffer (10u, 0xFF);
    
    const std::string kTestDir ("../Test/Files/tmp");
    ASSERT_TRUE (OS::FileSystem::CreateDir (kTestDir));
    ASSERT_TRUE (OS::FileSystem::Write (kTestDir + "/tmp1.txt", writeBuffer));
    ASSERT_TRUE (OS::FileSystem::Write (kTestDir + "/tmp2.txt", writeBuffer));
    ASSERT_TRUE (OS::FileSystem::Write (kTestDir + "/tmp3.txt", writeBuffer));
    
    std::vector<std::string> files;
    ASSERT_TRUE (OS::FileSystem::ListDir (kTestDir, files));
    ASSERT_EQ (std::string ("tmp1.txt"), files[0]);
    ASSERT_EQ (std::string ("tmp2.txt"), files[1]);
    ASSERT_EQ (std::string ("tmp3.txt"), files[2]);
    
    ASSERT_TRUE (OS::FileSystem::Remove (kTestDir));
    ASSERT_FALSE (OS::FileSystem::Exists (kTestDir));
}

TEST (FileSystemTester, GetExtension) {
    
    ASSERT_EQ (std::string (".txt"), OS::FileSystem::GetExtension ("/home/file.txt"));
    ASSERT_EQ (std::string (".txt"), OS::FileSystem::GetExtension ("file.txt"));
    ASSERT_EQ (std::string (".txt"), OS::FileSystem::GetExtension ("./file.txt"));
    ASSERT_EQ (std::string (".txt"), OS::FileSystem::GetExtension ("C:\\dir\file.txt"));
    ASSERT_EQ (std::string (".txt1"), OS::FileSystem::GetExtension ("file.txt1"));
    ASSERT_EQ (std::string (".Txt1"), OS::FileSystem::GetExtension ("file.Txt1"));
}

TEST (FileSystemTester, GetBasename) {
    
    ASSERT_EQ (std::string ("file"), OS::FileSystem::GetBasename ("/home/file.txt"));
    ASSERT_EQ (std::string ("file"), OS::FileSystem::GetBasename ("file.txt"));
    ASSERT_EQ (std::string ("file"), OS::FileSystem::GetBasename ("./file.txt"));
    ASSERT_EQ (std::string ("file"), OS::FileSystem::GetBasename ("C://dir/file.txt"));
}
