
#include "HttpRequestEncoder.h"

namespace {
    const std::string NEWLINE ("\r\n");
}

void HTTP::RequestEncoder::Write (const HTTP::Request& inRequest) {

    auto request (MethodToString (inRequest.mMethod) + std::string (" ") + 
        inRequest.mPath + std::string (" ") + 
        std::string ("HTTP/") + VersionToString (inRequest.mVersion) + NEWLINE
    );

    for (const auto& header : inRequest.GetAllHeaders ()) {
        request += header.GetKey () + std::string (": ") + header.GetValue () + std::string ("\r\n");
    }

    request += NEWLINE;
    request += inRequest.GetBody ();

    sNext.Emit (request);

}
