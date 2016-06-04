
#include "Network.h"
#include "Log.h"

#include "Router.h"
#include "Routes.h"

#include "TcpServer.h"
#include "SupportThread.h"

int main (int* argc, char** argv) {
    
    OS::Log::Instance ().Initialize (OS::Log::kTrace);
    OS::Network::Initialize ();

    APP::SupportThread supportThread;
    supportThread.Spawn ();

    auto router = std::make_shared <API::Router> ();
    Routes::SetupEndpoints (router);

    TCP::Server server ("127.0.0.1", "1234", router);
    server.Start ();

    supportThread.Join ();
    
    OS::Network::Done ();

    return 0;
}