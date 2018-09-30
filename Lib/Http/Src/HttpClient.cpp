
#include <iostream>
#include "Socket.h"
#include "Timing.h"
#include "Log.h"
#include "HttpClient.h"
#include "HttpResponse.h"
#include "HttpEndpoint.h"
#include "HttpRouter.h"

HTTP::Client::Client (std::unique_ptr <OS::Socket> inSocket, std::shared_ptr<HTTP::Router> inRouter) :
    TCP::Client (std::move (inSocket)),
    HTTP::RequestParser (),
    mRouter (inRouter)
{
}

HTTP::Client::~Client () = default;

void HTTP::Client::HandlePacket (const std::vector<uint8_t>& inBuffer) {
    const std::string input (reinterpret_cast<const char*>(inBuffer.data ()), inBuffer.size ());
    Write (input);
}

void HTTP::Client::HandleRequest (const HTTP::Request& inRequest) {
    Response response (Code::NOT_FOUND, inRequest.mVersion);
    mRouter->Dispatch (inRequest, response);
    response.mHeaders[Header::HOST] =  mSocket->GetAddress () + std::string (":") + mSocket->GetPortNumber ();
    SendResponse (inRequest, response);
}

void HTTP::Client::SendResponse (const HTTP::Request& inRequest, const HTTP::Response& inResponse) {
    LOGMESSAGE (OS::Log::kInfo, std::string ("HTTP/") + VersionToString (inResponse.mVersion) + std::string (" ") + MethodToString (inRequest.mMethod) + std::string (" ") + inRequest.mPath + std::string (" - ") + std::to_string (inResponse.mCode) + std::string (" ") + CodeToString (inResponse.mCode));
    const auto resStr (inResponse.ToString ());
    TCP::Client::Send (reinterpret_cast<const uint8_t*> (resStr.c_str ()), resStr.size ());
}

HTTP::ClientFactory::ClientFactory (std::shared_ptr<HTTP::Router> inRouter) :
    TCP::ClientFactory::ClientFactory (),
    mRouter (inRouter)
{
}

std::unique_ptr<TCP::Client> HTTP::ClientFactory::Create (std::unique_ptr<OS::Socket> inSocket) const {
    return std::make_unique<HTTP::Client> (std::move (inSocket), mRouter);
}
