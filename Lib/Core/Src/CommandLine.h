
#ifndef _COMMANDLINE_H_
#define _COMMANDLINE_H_

#include <string>
#include <vector>
#include <cstdint>

namespace OS {


/**
 * The CommandLine parser allows to parse command line
 * arguments and assign default values to them.
 */
class CommandLine {

public: 

    /**
     * Argument type.
     */
    enum Type : uint8_t {
        OPTIONAL = 0x00,
        MANDATORY =0x01
    };

    /**
     * Add a command line option.
     */
    void AddOption (const std::string& inKey, const std::string& inDefaultValue, Type inType = OPTIONAL);

    /**
     * Get the value of an option
     */
    std::string GetOption (const std::string& inKey) const;

    /**
     * Parse the command line. Returns false if not all MANDATORY arguments could be found.
     */
    bool Parse (int inArgc, const char* const* inArgv);

    /**
     * Retrieve the command line help text.
     */
    std::string GetHelpText () const;

private:

    /**
     * Command line options implementation
     */
    struct Option {
    public:
        Option (const std::string& inKey, const std::string& inValue, const Type inType);
        std::string mKey;
        std::string mValue;
        Type mType;
        bool mFound;
    };
    std::vector <Option> mOptions;
};

} // end namespace OS

#endif // _COMMANDLINE_H_