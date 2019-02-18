
#include <iostream>

#include "Mutex.h"
#include "WebSockClient.h"
#include "WebSockServer.h"

RFC6455::Server::Server (const std::string& inAddress, const std::string& inPort) :
    TCP::Server (inAddress, inPort, std::make_shared<RFC6455::ClientFactory> ())
{
}

RFC6455::Server::~Server () = default;

void RFC6455::Server::BroadCast (const std::string& inPacket) {
    
    OS::SingleLock lock (*mMutex);
    
    for (auto& client : mClients) {
        client->SendData (inPacket);
    }
}

void RFC6455::Server::BroadCast (const std::vector<uint8_t>& inPacket) {
    
    OS::SingleLock lock (*mMutex);
    
    for (auto& client : mClients) {
        client->SendData (inPacket);
    }
}
