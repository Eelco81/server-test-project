
#include <algorithm>
#include "CommandLine.h"
#include "Version.h"

OS::CommandLine::Option::Option (const std::string& inKey, const std::string& inValue, const Type inType):
    mKey (inKey),
    mValue (inValue),
    mType (inType),
    mFound (false)
{
}

void OS::CommandLine::AddOption (const std::string& inKey, const std::string& inDefaultValue, Type inType) {
    mOptions.push_back (Option (inKey, inDefaultValue, inType));
}

std::string OS::CommandLine::GetOption (const std::string& inKey) const {

    const auto equals ([inKey] (const Option& option) { return option.mKey == inKey; });
    const auto option (std::find_if (mOptions.begin (), mOptions.end (), equals));

    if (option != mOptions.end ()) {
        return (*option).mValue;
    }
    return "";
}

bool OS::CommandLine::Parse (int inArgc, const char* const* inArgv) {

    const char kIndicatorPrefix ('-');
    
    for (int i (0); i < inArgc; i+=1) {
    
        if (inArgv [i][0] == kIndicatorPrefix && i + 1 < inArgc) {
            
            std::string key (std::string (inArgv [i]).erase (0u, 1u));
            std::string value (inArgv [i+1]);
        
            const auto equals ([key] (const Option& option) { return option.mKey == key; });
            const auto option (std::find_if (mOptions.begin (), mOptions.end (), equals));
            
            if (option != mOptions.end ()) {
                (*option).mValue = value;
                (*option).mFound = true;
            }
        }
    }

    return std::all_of (mOptions.begin (), mOptions.end (), [](const auto& option) {
        return (option.mType == OPTIONAL) || (option.mType == MANDATORY && option.mFound);
    });
}

std::string OS::CommandLine::GetHelpText () const {
    
    std::string result;
    
    result += "-----------------------------------\n";
    result +=  " name    :    " + OS::Version::GetApplicationName() + "\n";
    result +=  " version :    " + OS::Version::GetApplicationVersion() + "\n";
    result += "-----------------------------------\n";
    
    for (const auto& option : mOptions) {
        result += std::string ("  -") + option.mKey + "\n";
        result += std::string ("    default : ") + option.mValue + std::string ("\n");
        result += std::string ("    type    : ") + std::string (option.mType == OPTIONAL ? "optional" : "mandatory") + std::string ("\n");
    }
    
    result += "-----------------------------------\n";
    
    return result;
}