
#include "gtest/gtest.h"

#include "CommandLine.h"

TEST (CommandLine, BasicParsing) {

    const int kArgc = 3;
    const char* kArgv [kArgc] = { "program_name", "-option1", "-option2" };

    OS::CommandLine commandLine;
    commandLine.Parse (kArgc, kArgv);

    ASSERT_TRUE (commandLine.HasOption ("option1"));
    ASSERT_TRUE (commandLine.HasOption ("option2"));
    ASSERT_FALSE (commandLine.HasOption ("program_name"));
    ASSERT_FALSE (commandLine.HasOption ("non_existing_option"));

}

TEST (CommandLine, KeyValueParsing) {

    const int kArgc (5);
    const char* kArgv [kArgc] = { "program_name", "-option1", "value1", "-option2", "value2"};

    OS::CommandLine commandLine;
    commandLine.Parse (kArgc, kArgv);

    std::string value;

    ASSERT_TRUE (commandLine.HasOption ("option1", value));
    ASSERT_FALSE (commandLine.HasOption ("value1"));

    ASSERT_TRUE (commandLine.HasOption ("option2", value));
    ASSERT_FALSE (commandLine.HasOption ("value1"));

}