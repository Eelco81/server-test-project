
#include <algorithm>
#include "CommandLine.h"

OS::CommandLine::Option::Option (const std::string& inKey) :
    mKey (inKey),
    mValue (""),
    mHasValue (false) 
{
}

OS::CommandLine::Option::Option (const std::string& inKey, const std::string& inValue) :
    mKey (inKey),
    mValue (inValue),
    mHasValue (true)
{
}

void OS::CommandLine::Parse (int inArgc, char** inArgv) {

    const char kIndicatorPrefix ('-');

    for (int i = 0; i < inArgc; i++) {
        
        if (inArgv [i][0] == kIndicatorPrefix) {
            if (i + 1 < inArgc && inArgv [i + 1][0] != kIndicatorPrefix) {
                std::string keyString (inArgv [i]);
                std::string valString (inArgv [i+1]);
                keyString.erase (0u, 1u);
                mOptions.emplace_back (Option (keyString, valString));
            }
            else {
                std::string keyString (inArgv [i]);
                keyString.erase (0u, 1u);
                mOptions.emplace_back (Option (keyString));
            }
        }
    }
}

bool OS::CommandLine::HasOption (const std::string& inKey) {
    Option option;
    if (FindOption (inKey, option)) {
        return true;
    }
    return false;
}

bool OS::CommandLine::HasOption (const std::string& inKey, std::string& outValue) {
    Option option;
    if (FindOption (inKey, option)) {
        if (option.mHasValue) {
            outValue = option.mValue;
        }
        return true;
    }
    return false;
}

bool OS::CommandLine::FindOption (const std::string& inKey, Option& outOption) {

    auto optionIterator = std::find_if (mOptions.begin (), mOptions.end (), [inKey] (const Option& option) { return option.mKey == inKey; });

    if (optionIterator != mOptions.end ()) {
        outOption = *optionIterator;
        return true;
    }

    return false;
}