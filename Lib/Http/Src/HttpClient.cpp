
#include <iostream>

#include "Socket.h"
#include "HttpClient.h"
#include "HttpRouter.h"

HTTP::Client::Client (std::unique_ptr <OS::Socket> inSocket, const HTTP::Router& inRouter) :
    TCP::Client (std::move (inSocket)),
    mRouter (inRouter)
{
    GetReadStream ()
        .Pipe (mToStringConverter)
        .Pipe (mDecoder)
        .Pipe (mRouter)
        .Pipe (mEncoder)
        .Pipe (mToPacketConverter)
        .Pipe (GetWriteStream ());
}

HTTP::Client::~Client () = default;

HTTP::ClientFactory::ClientFactory (const HTTP::Router& inRouter) :
    TCP::ClientFactory::ClientFactory (),
    mRouter (inRouter)
{
}

std::unique_ptr<TCP::Client> HTTP::ClientFactory::Create (std::unique_ptr<OS::Socket> inSocket) const {
    return std::make_unique<HTTP::Client> (std::move (inSocket), mRouter);
}
