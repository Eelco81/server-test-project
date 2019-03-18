
#ifndef _HTTP_CODES_H_
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

enum Code : uint32_t {
    UNKNOWN_CODE = 0x00,
    SWITCHING_PROTOCOLS = 101u,
    OK = 200u,
    CREATED = 201u,
    ACCEPTED = 202u,
    NO_CONTENT = 204u,
    NOT_MODIFIED = 304u,
    BAD_REQUEST = 400u,
    FORBIDDEN = 403u,
    NOT_FOUND = 404u,
    INTERNAL_SERVER_ERROR = 500u,
    NOT_IMPLEMENTED = 501u,
    HTTP_VERSION_NOT_SUPPORTED = 505u 
};

std::string CodeToString (Code inCode);
Code StringToCode (const std::string& inCode);

} // end namespace HTTP

#endif // _HTTP_REQUEST_H_
