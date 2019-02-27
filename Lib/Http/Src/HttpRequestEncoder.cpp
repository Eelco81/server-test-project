
#include "HttpRequestEncoder.h"

namespace {
    const std::string NEWLINE ("\r\n");
}

void HTTP::RequestEncoder::Write (const HTTP::Request& inRequest) {

    auto request (MethodToString (inRequest.mMethod) + std::string (" ") + 
        inRequest.mPath + std::string (" ") + 
        std::string ("HTTP/") + VersionToString (inRequest.mVersion) + NEWLINE
    );

    for (const auto& pair : inRequest.mHeaders) {
        request += pair.first + std::string (": ") + pair.second + std::string ("\r\n");
    }

    request += NEWLINE;
    request += inRequest.GetBody ();

    Emit (request);

}
