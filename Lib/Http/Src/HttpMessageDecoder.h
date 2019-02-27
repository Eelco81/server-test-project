
#ifndef _HTTP_MESSAGE_DECODER_H_
#define _HTTP_MESSAGE_DECODER_H_

#include <memory>
#include <regex>
#include <iostream>
#include <sstream>
#include <string>
#include "MessageStream.h"

namespace HTTP {

/**
 * The HTTP::MessageDecoder is the base for the RequestDecoder and ResponseEncoder.
 */
template<class Message_t>
class MessageDecoder : public OS::MessageStream<std::string, Message_t> {

public:
    /**
     * Constructor
     */
    MessageDecoder ();
    
    /**
     * Virtual destructor
     */
    virtual ~MessageDecoder ();
    
    /**
     * Write a string to the Decoder, when a corresponding Message_t is assembled,
     * the MessageStream::Emit method is called.
     */ 
    void Write (const std::string& inData);

    /**
     * To be overloaded function. Important, if this line matches
     * the message startline, the implemented function MUST reset mMessage!!
     */
    virtual bool MatchStartLine (const std::string& inLine) = 0;

protected:
    /**
     * The request being assembled
     */
    std::unique_ptr<Message_t> mMessage;
    
    /**
     * State of the decoder
     */
    enum State : uint8_t {
        kSearchStart = 0x00,
        kProcessHeaders = 0x01,
        kProcessBody = 0x02
    };
    State mState;
    
    /**
     * The body size of the currently assembled request.
     */
    std::size_t mBodySize;
    std::string mBody;
};

} // end namespace HTTP

#include "HttpMessageDecoder.tcc"

#endif // _HTTP_MESSAGE_DECODER_H_
