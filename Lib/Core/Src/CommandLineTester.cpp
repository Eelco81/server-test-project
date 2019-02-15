
#include "gtest/gtest.h"
#include "CommandLine.h"

TEST (CommandLine, OptionsRetrieval) {

    OS::CommandLine commandLine;
    commandLine.AddOption ("key1", "default", OS::CommandLine::MANDATORY);

    const int kArgc = 3;
    const char* const kArgv [kArgc] = { "program_name", "-key1", "value1" };
    
    ASSERT_TRUE (commandLine.Parse (kArgc, kArgv));
    
    ASSERT_EQ (std::string ("value1"), commandLine.GetOption ("key1"));
    ASSERT_EQ (std::string (""), commandLine.GetOption ("key2"));
}

TEST (CommandLine, MissingMandatoryOptions) {

    OS::CommandLine commandLine;
    commandLine.AddOption ("key1", "default1", OS::CommandLine::MANDATORY);
    
    const int kArgc = 3;
    const char* const kArgv [kArgc] = { "program_name", "-key2", "value2" };
    
    ASSERT_FALSE (commandLine.Parse (kArgc, kArgv));
}

TEST (CommandLine, DefaultValues) {

    OS::CommandLine commandLine;
    commandLine.AddOption ("key1", "default1", OS::CommandLine::OPTIONAL);
    
    const int kArgc = 3;
    const char* const kArgv [kArgc] = { "program_name", "-key2", "value2" };
    
    ASSERT_TRUE (commandLine.Parse (kArgc, kArgv));
    
    ASSERT_EQ (std::string ("default1"), commandLine.GetOption ("key1"));
}

TEST (CommandLine, HelpText) {
    
    OS::CommandLine commandLine;
    commandLine.AddOption ("key1", "default1", OS::CommandLine::OPTIONAL);
    commandLine.AddOption ("key2", "default2", OS::CommandLine::MANDATORY);
    commandLine.AddOption ("key3", "default3", OS::CommandLine::OPTIONAL);
    
    ASSERT_EQ ("-----------------------------------\n name    :    Unknown\n version :    0.0.1\n-----------------------------------\n  -key1\n    default : default1\n    type    : optional\n  -key2\n    default : default2\n    type    : mandatory\n  -key3\n    default : default3\n    type    : optional\n-----------------------------------\n", commandLine.GetHelpText ());
}
