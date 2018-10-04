
#include <iostream>

#include "Network.h"
#include "Log.h"
#include "Version.h"
#include "CommandLine.h"
#include "Socket.h"
#include "HttpClient.h"
#include "WebSockClient.h"
#include "SystemRouter.h"
#include "TcpServer.h"
#include "SupportThread.h"

int main (int argc, char** argv) {
    
    OS::CommandLine commandLine;
    commandLine.Parse (argc, argv);

    std::string ip ("127.0.0.1");
    commandLine.HasOption ("ip", ip);

    std::string port ("1703");
    commandLine.HasOption ("port", port);
    
    std::string websockport ("1704");
    commandLine.HasOption ("websockport", websockport);

    std::string logLevel ("INFO");
    commandLine.HasOption ("loglevel", logLevel);

    LOGMESSAGE (OS::Log::kInfo, std::string ("Starting ") + OS::Version::GetApplicationName () + std::string (" ") + OS::Version::GetApplicationVersion () );
    
    OS::Log::Instance ().Initialize (logLevel);
    OS::Network::Initialize ();

    APP::SupportThread supportThread;
    supportThread.Spawn ();

    auto router (std::make_shared<API::SystemRouter> ());
    auto httpFactory (std::make_shared<HTTP::ClientFactory> (router));
    TCP::Server httpServer (ip, port, httpFactory);
    httpServer.Start ();
    
    auto websockFactory (std::make_shared<RFC6455::ClientFactory> ());
    TCP::Server websockServer (ip, websockport, websockFactory);
    websockServer.Start ();

    std::string name;
    LOGMESSAGE (OS::Log::kInfo, "Quit the app using ENTER on the command line.");
    std::getline (std::cin, name);
    
    websockServer.Stop ();
    httpServer.Stop ();

    supportThread.Kill ();
    supportThread.Join ();
    
    OS::Network::Done ();

    OS::Log::Instance ().Flush ();
    
    return 0;
}
