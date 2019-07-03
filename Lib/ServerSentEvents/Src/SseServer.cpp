
#include <iostream>

#include "Mutex.h"
#include "SseClient.h"
#include "SseServer.h"

SSE::Server::Server (const std::string& inAddress, const std::string& inPort) :
    TCP::Server (inAddress, inPort, std::make_shared<SSE::ClientFactory> ())
{
}

SSE::Server::~Server () = default;

void SSE::Server::BroadCast (const std::string& inPacket) {
    
    OS::SingleLock lock (*mMutex);
    
    for (auto& client : mClients) {
        dynamic_cast<SSE::Client*> (client.get ())->SendPayload (inPacket);
    }
}
