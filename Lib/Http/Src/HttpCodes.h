

#ifndef _HTTP_CODES_H
#define _HTTP_CODES_H_

#include <cstdint>
#include <string>

namespace HTTP {
    
enum Method : uint8_t {
    UNKNOWN_METHOD = 0x00,
    GET = 0x01,
    POST = 0x02,
    PUT = 0x03,
    DELETE = 0x04
};

std::string MethodToString (Method inMethod);
Method StringToMethod (const std::string& inString);

enum Version : uint8_t {
    UNKNOWN_VERSION = 0x00,
    V10 = 0x01,
    V11 = 0x02
};

std::string VersionToString (Version inVersion);
Version StringToVersion (const std::string& inString);

enum ResponseCode : uint32_t {
    OK = 200u,
    CLIENT_ERROR = 400u,
    NOT_FOUND = 404u,
    SERVER_ERROR = 500u
};

} // end namespace HTTP

#endif // _HTTP_REQUEST_H_
