
#include "HttpMessage.h"
#include "HttpCodes.h"

HTTP::Message::Message ()
{
    mHeaders[Header::CONTENT_LENGTH] = "0";
}

void HTTP::Message::SetBody (const std::string& inBody, const std::string& inContentType) {
    mBody = inBody;
    mHeaders[HTTP::Header::CONTENT_LENGTH] = std::to_string (mBody.size ());
    if (mBody.size () > 0u) {
        mHeaders[HTTP::Header::CONTENT_TYPE] = inContentType;
    }
    else {
        mHeaders.erase (HTTP::Header::CONTENT_TYPE);
    }
}

void HTTP::Message::SetBody (const std::vector<uint8_t>& inBuffer, const std::string& inContentType) {
    mBody.assign (reinterpret_cast<const char*> (inBuffer.data ()), inBuffer.size ());
    mHeaders[HTTP::Header::CONTENT_LENGTH] = std::to_string (mBody.size ());
    if (mBody.size () > 0u) {
        mHeaders[HTTP::Header::CONTENT_TYPE] = inContentType;
    }
    else {
        mHeaders.erase (HTTP::Header::CONTENT_TYPE);
    }
}

const std::string& HTTP::Message::GetBody () const {
    return mBody;
}
