
#include <iostream>
#include <string>

#include "ServerApplication.h"

#include "Log.h"
#include "Socket.h"
#include "HttpClient.h"
#include "WebSockServer.h"
#include "TcpServer.h"
#include "SupportThread.h"
#include "SimService.h"
#include "SimComFactory.h"
#include "Router.h"

ServerApplication::ServerApplication (int argc, char** argv) :
    APP::Application (argc, argv, "Server") 
{
}

ServerApplication::~ServerApplication () = default;

int ServerApplication::Run () {
    
    std::string ip ("127.0.0.1");
    mCommandLine.HasOption ("ip", ip);

    std::string port ("1703");
    mCommandLine.HasOption ("port", port);

    std::string websockport ("1704");
    mCommandLine.HasOption ("websockport", websockport);

    auto service = std::make_shared<SIM::Service> (std::make_unique<SIM::COM::Factory> ());

    SystemRouter router (service);
    auto httpFactory (std::make_shared<HTTP::ClientFactory> (router));
    TCP::Server httpServer (ip, port, httpFactory);
    httpServer.Start ();

    RFC6455::Server websockServer (ip, websockport);
    websockServer.Start ();

    service->GetStream ().Pipe (&websockServer, &RFC6455::Server::BroadCast);
    
    std::string name;
    LOGMESSAGE (OS::Log::kInfo, "Quit the app using ENTER on the command line.");
    std::getline (std::cin, name);

    // Cannot rely on destructors to clean this up. The service is a shared_ptr
    // and will be cleaned up all the way at the end.
    if (service->IsRunning ()) service->Stop ();
    
    return 0;
}
