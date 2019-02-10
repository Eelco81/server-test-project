
#include "HttpCodes.h"

std::string HTTP::MethodToString (HTTP::Method inMethod) {
    switch (inMethod) {
        case Method::GET : return "GET";
        case Method::PUT : return "PUT";
        case Method::POST : return "POST";
        case Method::DELETE : return "DELETE";
        default : return "-";
    }
}

HTTP::Method HTTP::StringToMethod (const std::string& inString) {
    if (inString == "GET") return Method::GET;
    if (inString == "PUT") return Method::PUT;
    if (inString == "POST") return Method::POST;
    if (inString == "DELETE") return Method::DELETE;
    return Method::UNKNOWN_METHOD;
}

std::string HTTP::VersionToString (HTTP::Version inVersion) {
    switch (inVersion) {
        case Version::V10 : return "1.0";
        case Version::V11 : return "1.1";
        default : return "x.x";
    }
}

HTTP::Version HTTP::StringToVersion (const std::string& inString) {
    if (inString == "1.0") return Version::V10;
    if (inString == "1.1") return Version::V11;
    return Version::UNKNOWN_VERSION;
}

std::string HTTP::CodeToString (HTTP::Code inCode) {
    switch (inCode) {
        case Code::SWITCHING_PROTOCOLS : return "Switching Protocols";
        case Code::OK : return "OK";
        case Code::CREATED : return "Created";
        case Code::ACCEPTED : return "Accepted";
        case Code::NO_CONTENT : return "No Content";
        case Code::NOT_MODIFIED : return "Not Modified";
        case Code::BAD_REQUEST : return "Bad Request";
        case Code::FORBIDDEN : return "Forbidden";
        case Code::NOT_FOUND : return "Not Found";
        case Code::INTERNAL_SERVER_ERROR : return "Internal Server Error";
        case Code::NOT_IMPLEMENTED : return "Not Implemented";
        case Code::HTTP_VERSION_NOT_SUPPORTED : return "HTTP Version Not Supported";
        default : return "Unknown";
    }
}

HTTP::Code HTTP::StringToCode (const std::string& inCode) {
    return static_cast<Code> (atoi (inCode.c_str ()));
}
