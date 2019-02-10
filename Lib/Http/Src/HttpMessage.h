
#ifndef _HTTP_MESSAGE_H_
#define _HTTP_MESSAGE_H_

#include <map>
#include <string>
#include <vector>
#include <cstdint>

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
     * \todo: header keys are not case sensitive
     */
    using HeaderMap = std::map<std::string,std::string>;
    HeaderMap mHeaders; 
    
    /**
     * Internal boolean used by the message decoder.
     */
    bool mIsValid;
    
protected:
    /**
     * HTTP body
     */
    std::string mBody;
    
};

} // end namespace HTTP

#endif // _HTTP_MESSAGE_H_
