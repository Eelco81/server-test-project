
#ifndef _HTTP_MESSAGE_H_
#define _HTTP_MESSAGE_H_

#include <string>
#include <vector>
#include <cstdint>
#include "HttpHeader.h"

namespace HTTP {

/**
 * The HTTP::Message struct is the base for the HTTP::Request and HTTP:Response structs.
 */
struct Message {
    
    /**
     * Constructor, initializes body and header map.
     */
    Message ();
    
    /**
     * Set the message body 
     */
    void SetBody (const std::string& inBody, const std::string& inContentType = "text/plain");
    void SetBody (const std::vector<uint8_t>& inBuffer, const std::string& inContentType = "application/octet-stream");
    
    /**
     * Get the message body
     */
    const std::string& GetBody () const;
    
    /**
     * HTTP headers 
     */
    using HeaderMap = std::vector<Header>;
    
    /**
     * Get Headers corresponding to inKey
     */
    HeaderMap GetHeaders (const std::string& inKey) const;
    
    /**
     * Get Headers corresponding to inKey (stop at first match)
     */
    std::string GetHeaderValue (const std::string& inKey) const;
    
    /**
     * Get Headers corresponding to inKey
     */
    const HeaderMap& GetAllHeaders () const;
    
    /**
     * Set header to the list (overwrites existing value).
     */
    void SetHeader (const Header& inHeader);
    
    /**
     * Add header to the list.
     */
    void AddHeader (const Header& inHeader);
    
    /**
     * Remove header to the list.
     */
    void RemoveHeaders (const std::string& inKey);
    
    /**
     * Internal boolean used by the message decoder.
     */
    bool mIsValid;
    
protected:
    /**
     * HTTP body
     */
    HeaderMap mHeaders;
    std::string mBody; 
    
};

} // end namespace HTTP

#endif // _HTTP_MESSAGE_H_
