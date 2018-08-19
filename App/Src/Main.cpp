
#include <iostream>

#include "Network.h"
#include "Log.h"
#include "Version.h"
#include "CommandLine.h"
#include "Socket.h"
#include "HttpClient.h"
#include "SystemRouter.h"
#include "TcpServer.h"
#include "SupportThread.h"

int main (int argc, char** argv) {
    
    OS::CommandLine commandLine;
    commandLine.Parse (argc, argv);

    std::string ip ("127.0.0.1");
    commandLine.HasOption ("ip", ip);

    std::string port ("1234");
    commandLine.HasOption ("port", port);

    std::string logLevel ("INFO");
    commandLine.HasOption ("loglevel", logLevel);

    LOGMESSAGE (OS::Log::kInfo, std::string ("Starting ") + OS::Version::GetApplicationName () + std::string (" ") + OS::Version::GetApplicationVersion () );
    
    OS::Log::Instance ().Initialize (logLevel);
    OS::Network::Initialize ();

    APP::SupportThread supportThread;
    supportThread.Spawn ();

    auto router (std::make_shared<API::SystemRouter> ());
    auto factory (std::make_shared<HTTP::ClientFactory> (router));
    TCP::Server server (ip, port, factory);
    server.Start ();

    std::string name;
    LOGMESSAGE (OS::Log::kInfo, "Quit the app using ENTER on the command line.");
    std::getline (std::cin, name);
    
    server.Stop ();

    supportThread.Kill ();
    supportThread.Join ();
    
    OS::Network::Done ();

    OS::Log::Instance ().Flush ();
    
    return 0;
}
