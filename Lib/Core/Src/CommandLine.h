
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
     * Add a command line option with multiple keys
     */
    void AddOption (const std::vector<std::string>& inKeys, const std::string& inDefaultValue, Type inType = OPTIONAL);

    /**
     * Get the value of an option
     */
    std::string GetOption (const std::string& inKey);

    /**
     * Verify of the command line contained an option
     */
    bool ContainsOption (const std::string& inKey);
    
    /**
     * Parse the command line. Returns false if not all MANDATORY arguments could be found.
     */
    bool Parse (int inArgc, const char* const* inArgv);

    /**
     * Retrieve the command line help text.
     */
    std::string GetHelpText () const;

    /**
     * Retrieve the command line version text
     */
    std::string GetVersionText () const;
    
private:

    /**
     * Command line options implementation
     */
    struct Option {
    public:
        Option (const std::vector<std::string>& inKey, const std::string& inValue, const Type inType);
        std::vector<std::string> mKeys;
        std::string mValue;
        Type mType;
        bool mFound;
    };
    std::vector <Option> mOptions;

    /**
     * Internal utility function
     */
    auto FindOption (const std::string& inKey);
};

} // end namespace OS

#endif // _COMMANDLINE_H_
