
#include "HttpResponseEncoder.h"

namespace {
    const std::string NEWLINE ("\r\n");
}

void HTTP::ResponseEncoder::Write (const HTTP::Response& inResponse) {

    auto response (std::string ("HTTP/") + VersionToString (inResponse.mVersion) + 
        std::string (" ") + std::to_string (inResponse.mCode) + std::string (" ") +
        CodeToString (inResponse.mCode) + NEWLINE
    );

    for (const auto& header : inResponse.GetAllHeaders ()) {
        response += header.GetKey () + std::string (": ") + header.GetValue () + NEWLINE;
    }

    response += NEWLINE;
    response += inResponse.GetBody ();

    sNext.Emit (response);

}
