
#include "Network.h"
#include "Log.h"
#include "CommandLine.h"
#include "Router.h"
#include "Routes.h"

#include "TcpServer.h"
#include "SupportThread.h"

int main (int argc, char** argv) {
    
    OS::Log::Instance ().Initialize (OS::Log::kTrace);
    OS::Network::Initialize ();

    OS::CommandLine commandLine;
    commandLine.Parse (argc, argv);

    std::string ip ("127.0.0.1");
    commandLine.HasOption ("ip", ip);

    std::string port ("1234");
    commandLine.HasOption ("port", port);

    APP::SupportThread supportThread;
    supportThread.Spawn ();

    auto router = std::make_shared <API::Router> ();
    Routes::SetupEndpoints (router);

    TCP::Server server (ip, port, router);
    server.Start ();

    supportThread.Join ();
    
    OS::Network::Done ();

    return 0;
}