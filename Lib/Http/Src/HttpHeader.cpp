
#include <algorithm>
#include "HttpHeader.h"

namespace {

/**
 * Set a string to lowercase
 */
std::string ToLowerCase (const std::string& inKey) {
    std::string result;
    std::transform (inKey.begin (), inKey.end (), std::back_inserter (result), ::tolower);
    return result;
}

} // end anonymous namespace

const std::string HTTP::Header::CONTENT_LENGTH = "Content-Length";
const std::string HTTP::Header::CONTENT_TYPE = "Content-Type";
const std::string HTTP::Header::USER_AGENT = "User-Agent";
const std::string HTTP::Header::LAST_MODIFIED = "Last-Modified";
const std::string HTTP::Header::HOST = "Host";

HTTP::Header::Header (const std::string& inKey, const std::string& inValue) :
    mKey (ToLowerCase (inKey)),
    mValue (inValue)
{
}

const std::string& HTTP::Header::GetKey () const {
    return mKey;
}

void HTTP::Header::SetKey (const std::string& inKey) {
    mKey = ToLowerCase (inKey);
}

bool HTTP::Header::MatchesKey (const std::string& inKey) const {
    return mKey == ToLowerCase (inKey);
}

const std::string& HTTP::Header::GetValue () const {
    return mValue;
}

void HTTP::Header::SetValue (const std::string& inValue) {
    mValue = inValue;
}
