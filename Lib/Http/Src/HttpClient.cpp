
#include <iostream>
#include "Socket.h"
#include "Timing.h"
#include "Log.h"
#include "HttpClient.h"
#include "HttpResponse.h"

HTTP::Client::Client (std::string inAddress, std::string inPort) :
    TCP::Client (inAddress, inPort)
{
}

HTTP::Client::Client (std::unique_ptr <OS::Socket> inSocket) :
    TCP::Client (std::move (inSocket))
{
}

void HTTP::Client::OnReceived (const std::vector<uint8_t>& inBuffer) {
    const std::string input (reinterpret_cast<const char*>(inBuffer.data ()), inBuffer.size());
    Write (input);
}

void HTTP::Client::HandleRequest (const HTTP::Request& inRequest) {
    Response response (Code::NOT_FOUND, inRequest.mVersion);
    response.mHeaders[Header::HOST] = "127.0.0.1:1234";
    if (inRequest.mVersion != Version::V11) {
        response.mCode = Code::HTTP_VERSION_NOT_SUPPORTED;
    }
    SendResponse (inRequest, response);
}

void HTTP::Client::SendResponse (const HTTP::Request& inRequest, const HTTP::Response& inResponse) {
    LOGMESSAGE (OS::Log::kInfo, std::string ("HTTP/") + VersionToString (inResponse.mVersion) + std::string (" ") + MethodToString (inRequest.mMethod) + std::string (" ") + inRequest.mPath + std::string (" - ") + std::to_string (inResponse.mCode) + std::string (" ") + CodeToString (inResponse.mCode));
    const auto resStr (inResponse.ToString ());
    TCP::Client::Send (reinterpret_cast<const uint8_t*> (resStr.c_str()), resStr.size());
}

std::unique_ptr<TCP::Client> HTTP::ClientFactory::Create (std::unique_ptr<OS::Socket> inSocket) const {
    return std::make_unique<HTTP::Client> (std::move (inSocket));
}
