
#ifndef _COMMANDLINE_H_
#define _COMMANDLINE_H_

#include <string>
#include <vector>

namespace OS {

class CommandLine {

public: 

    void Parse (int inArgc, const char* const* inArgv);
    bool HasOption (const std::string& inKey) const;
    bool HasOption (const std::string& inKey, std::string& outValue) const;

private:
    
    struct Option {
        Option () = default;
        Option (const Option& inOption) = default;
        ~Option () = default;
        Option (const std::string& inKey);
        Option (const std::string& inKey, const std::string& inValue);
        std::string mKey;
        std::string mValue;
        bool mHasValue;
    };

    std::vector <Option> mOptions;

    bool FindOption (const std::string& inKey, Option& outOption) const;
};

}

#endif