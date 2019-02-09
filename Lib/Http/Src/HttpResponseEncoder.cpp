
#include "HttpResponseEncoder.h"

namespace {
    const std::string NEWLINE ("\r\n");
}

void HTTP::ResponseEncoder::Write (const HTTP::Response& inResponse) {

    auto response (std::string ("HTTP/") + VersionToString (inResponse.mVersion) + 
        std::string (" ") + std::to_string (inResponse.mCode) + std::string (" ") +
        CodeToString (inResponse.mCode) + NEWLINE
    );

    for (const auto& pair : inResponse.mHeaders) {
        response += pair.first + std::string (": ") + pair.second + NEWLINE;
    }

    response += NEWLINE;
    response += inResponse.GetBody ();

    Done (response);

}
