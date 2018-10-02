
#include <iostream>
#include "Socket.h"
#include "Timing.h"
#include "Log.h"
#include "HttpClient.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpEndpoint.h"
#include "HttpRouter.h"

HTTP::Client::Client (std::unique_ptr <OS::Socket> inSocket, std::shared_ptr<HTTP::Router> inRouter) :
    TCP::Client (std::move (inSocket)),
    mRouter (inRouter)
{
    GetReadStream ().Pipe (mToStringConverter).Pipe (mDecoder).Pipe (this, &Client::HandleRequest);
    mEncoder.Pipe (mToPacketConverter).Pipe (GetWriteStream ());
}

HTTP::Client::~Client () = default;

void HTTP::Client::HandleRequest (const HTTP::Request& inRequest) {
    Response response (Code::NOT_FOUND, inRequest.mVersion);
    mRouter->Dispatch (inRequest, response);
    response.mHeaders[Header::HOST] =  mSocket->GetAddress () + std::string (":") + mSocket->GetPortNumber ();
    mEncoder.Write (response);
    LOGMESSAGE (OS::Log::kInfo, std::string ("HTTP/") + VersionToString (response.mVersion) + std::string (" ") + MethodToString (inRequest.mMethod) + std::string (" ") + inRequest.mPath + std::string (" - ") + std::to_string (response.mCode) + std::string (" ") + CodeToString (response.mCode));
}

HTTP::ClientFactory::ClientFactory (std::shared_ptr<HTTP::Router> inRouter) :
    TCP::ClientFactory::ClientFactory (),
    mRouter (inRouter)
{
}

std::unique_ptr<TCP::Client> HTTP::ClientFactory::Create (std::unique_ptr<OS::Socket> inSocket) const {
    return std::make_unique<HTTP::Client> (std::move (inSocket), mRouter);
}
