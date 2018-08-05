
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
        default : return "-";
    }
}

HTTP::Version HTTP::StringToVersion (const std::string& inString) {
    if (inString == "1.0") return Version::V10;
    if (inString == "1.1") return Version::V11;
    return Version::UNKNOWN_VERSION;
}
