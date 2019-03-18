
#include <iostream>
#include <algorithm>
#include "HttpMessage.h"
#include "HttpCodes.h"

HTTP::Message::Message () :
    mIsValid (true)
{
    SetHeader (Header (Header::CONTENT_LENGTH, "0"));
}

void HTTP::Message::SetBody (const std::string& inBody, const std::string& inContentType) {
    mBody = inBody;
    SetHeader (Header (Header::CONTENT_LENGTH, std::to_string (mBody.size ())));
    if (mBody.size () > 0u) {
        SetHeader (Header (HTTP::Header::CONTENT_TYPE, inContentType));
    }
    else {
        RemoveHeaders (HTTP::Header::CONTENT_TYPE);
    }
}

void HTTP::Message::SetBody (const std::vector<uint8_t>& inBuffer, const std::string& inContentType) {
    mBody.assign (reinterpret_cast<const char*> (inBuffer.data ()), inBuffer.size ());
    SetHeader (Header (Header::CONTENT_LENGTH, std::to_string (mBody.size ())));
    if (mBody.size () > 0u) {
        SetHeader (Header (HTTP::Header::CONTENT_TYPE, inContentType));
    }
    else {
        RemoveHeaders (HTTP::Header::CONTENT_TYPE);
    }
}

const std::string& HTTP::Message::GetBody () const {
    return mBody;
}

HTTP::Message::HeaderMap HTTP::Message::GetHeaders (const std::string& inKey) const {
    HeaderMap headers;
    for (const auto& header : mHeaders) {
        if (header.MatchesKey (inKey)) {
            headers.push_back (header);
        }
    }
    return headers;
}

std::string HTTP::Message::GetHeaderValue (const std::string& inKey) const {
    for (const auto& header : mHeaders) {
        if (header.MatchesKey (inKey)) {
            return header.GetValue ();
        }
    }
    return "";
}

const HTTP::Message::HeaderMap& HTTP::Message::GetAllHeaders () const {
    return mHeaders;
}

void HTTP::Message::SetHeader (const HTTP::Header& inHeader) {
    for (auto& header : mHeaders) {
        if (header.MatchesKey (inHeader.GetKey ())) {
            header.SetValue (inHeader.GetValue ());
            return;
        }
    }
    AddHeader (inHeader);
}

void HTTP::Message::AddHeader (const HTTP::Header& inHeader) {
    mHeaders.push_back (inHeader);
}

void HTTP::Message::RemoveHeaders (const std::string& inKey) {
    const auto eraser = [inKey](const auto& inHeader) {
        return inHeader.MatchesKey (inKey);
    };
    mHeaders.erase (std::remove_if (mHeaders.begin (), mHeaders.end (), eraser));
}
