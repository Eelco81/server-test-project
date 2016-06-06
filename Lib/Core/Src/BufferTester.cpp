
#include "gtest/gtest.h"

#include <string>
#include <string.h>

#include "Buffer.h"

TEST (BufferTester, InitBufferSize) {

    OS::Buffer buffer (5u);

    ASSERT_EQ (0u, buffer.GetSize ());
    ASSERT_EQ (5u, buffer.GetMaxSize ());
}

TEST (BufferTester, SetBufferValues) {

    const char* kTestData = "Some Other Data";
    const unsigned kTestSize (static_cast<unsigned> (strlen (kTestData)));

    OS::Buffer buffer (20u);

    buffer.SetData (kTestData, kTestSize);
    ASSERT_EQ (kTestSize, buffer.GetSize ());

    std::string output;
    buffer.ToString (output);
    ASSERT_EQ (std::string (kTestData), output);
}

TEST (BufferTester, ChangingBufferSizes) {

    const char* kTestData = "12345";
    const unsigned kTestSize = static_cast<unsigned>(strlen (kTestData));

    OS::Buffer buffer (kTestSize);

    ASSERT_EQ (0u, buffer.GetSize ());
    ASSERT_EQ (kTestSize, buffer.GetMaxSize ());

    buffer.SetData (kTestData, kTestSize);
    ASSERT_EQ (kTestSize, buffer.GetSize ());

    std::string output ("");
    buffer.ToString (output);
    ASSERT_EQ (std::string (kTestData), output);

    buffer.Resize (6u);
    ASSERT_EQ (6u, buffer.GetMaxSize ());
    ASSERT_EQ (6u, buffer.GetSize ());

    buffer.ToString (output);
    ASSERT_EQ (std::string ("12345\xCD"), output);

    buffer.Resize (4u);
    ASSERT_EQ (6u, buffer.GetMaxSize ());
    ASSERT_EQ (4u, buffer.GetSize ());

    buffer.ToString (output);
    ASSERT_EQ (std::string ("1234"), output);
}
