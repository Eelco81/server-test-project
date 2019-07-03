
#include <iostream>

#include "Socket.h"
#include "HttpClient.h"
#include "HttpRouter.h"

HTTP::Client::Client (std::unique_ptr <OS::Socket> inSocket, const HTTP::Router& inRouter) :
    TCP::Client (std::move (inSocket)),
    mRouter (inRouter)
{
    Pipe (sDataAvailable, mPacket2String)
        .Pipe (mDecoder)
        .Pipe (mRouter)
        .Pipe (mEncoder)
        .Pipe (mString2Packet)
        .Pipe<TCP::Client> (this, &TCP::Client::Send);
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
