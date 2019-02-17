
#include <algorithm>
#include "CommandLine.h"
#include "Version.h"

OS::CommandLine::Option::Option (const std::vector<std::string>& inKeys, const std::string& inValue, const Type inType):
    mKeys (inKeys),
    mValue (inValue),
    mType (inType),
    mFound (false)
{
}

auto OS::CommandLine::FindOption (const std::string& inKey) {
    const auto equals ([inKey] (const Option& option) { 
        return std::any_of (option.mKeys.begin(), option.mKeys.end (), [inKey](const auto& key){ 
            return key == inKey; 
        });
    });
    return std::find_if (mOptions.begin (), mOptions.end (), equals);
}

void OS::CommandLine::AddOption (const std::vector<std::string>& inKeys, const std::string& inDefaultValue, Type inType) {
    mOptions.push_back (Option (inKeys, inDefaultValue, inType));
}

std::string OS::CommandLine::GetOption (const std::string& inKey) {

    auto option (FindOption (inKey));
    if (option != mOptions.end ()) {
        return (*option).mValue;
    }
    return "";
}

bool OS::CommandLine::ContainsOption (const std::string& inKey) {

    auto option (FindOption (inKey));
    if (option != mOptions.end ()) {
        return (*option).mFound;
    }
    return false;
}

bool OS::CommandLine::Parse (int inArgc, const char* const* inArgv) {

    const char kIndicatorPrefix ('-');
    
    for (int i (0); i < inArgc; i+=1) {
        
        if (inArgv [i][0] == kIndicatorPrefix) {
            
            if (i + 1 < inArgc && inArgv [i+1][0] != kIndicatorPrefix) {
                std::string key (std::string (inArgv [i]).erase (0u, 1u));
                std::string value (inArgv [i+1]);
                
                auto option (FindOption (key));
                if (option != mOptions.end ()) {
                    (*option).mValue = value;
                    (*option).mFound = true;
                }
            }
            else {
                std::string key (std::string (inArgv [i]).erase (0u, 1u));
                
                auto option (FindOption (key));
                if (option != mOptions.end ()) {
                    (*option).mFound = true;
                }
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
    result +=  " name    :    " + OS::Version::GetApplicationName () + "\n";
    result +=  " version :    " + OS::Version::GetApplicationVersion () + "\n";
    result += "-----------------------------------\n";
    
    for (const auto& option : mOptions) {
        for (const auto& key : option.mKeys) {
            result += std::string ("-") + key + std::string (" ");
        }
        result += "\n";
        result += std::string ("    default : ") + option.mValue + std::string ("\n");
        result += std::string ("    type    : ") + std::string (option.mType == OPTIONAL ? "optional" : "mandatory") + std::string ("\n");
    }
    
    result += "-----------------------------------\n";
    
    return result;
}

std::string OS::CommandLine::GetVersionText () const {
    return OS::Version::GetApplicationName () + " : " + OS::Version::GetApplicationVersion ();
}
