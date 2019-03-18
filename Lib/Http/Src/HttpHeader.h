
#ifndef _HTTP_HEADER_H_
#define _HTTP_HEADER_H_

#include <string>

namespace HTTP {

/**
 * The HTTP::Header class is a container for the 
 * key-value pair.
 */
class Header {
    

public:
    /**
     * Default headers
     */
    static const std::string CONTENT_LENGTH;
    static const std::string CONTENT_TYPE;
    static const std::string USER_AGENT;
    static const std::string LAST_MODIFIED;
    static const std::string HOST;
    
    /**
     * Constructors
     */
    Header () = default;
    Header (const Header&) = default;
    Header (const std::string& inKey, const std::string& inValue);
    
    /**
     * Get / Set the key. Note that the key is always stored
     * in all lowercase.
     */
    const std::string& GetKey () const;
    void SetKey (const std::string& inKey);
    
    /**
     * Verify if the key matches.
     */
    bool MatchesKey (const std::string& inKey) const;
    
    /**
     * Get / Set the value
     */
    const std::string& GetValue () const;
    void SetValue (const std::string& inValue);
    
private:
    std::string mKey;
    std::string mValue;
    
}; // end Header

} // end namespace HTTP

#endif // _HTTP_HEADER_H_
