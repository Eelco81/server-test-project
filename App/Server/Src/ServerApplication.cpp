
#include <iostream>
#include <string>

#include "ServerApplication.h"

#include "Log.h"
#include "Socket.h"
#include "HttpClient.h"
#include "WebSockServer.h"
#include "SseServer.h"
#include "TcpServer.h"
#include "SupportThread.h"
#include "SimService.h"
#include "SimComFactory.h"
#include "Router.h"

ServerApplication::ServerApplication () :
    APP::Application ("Server") 
{
    mCommandLine.AddOption ({"ip", "i"}, "127.0.0.1", OS::CommandLine::OPTIONAL);
    mCommandLine.AddOption ({"port", "p"}, "1703", OS::CommandLine::OPTIONAL);
    mCommandLine.AddOption ({"websockport", "w"}, "1704", OS::CommandLine::OPTIONAL);
    mCommandLine.AddOption ({"sseport", "w"}, "1705", OS::CommandLine::OPTIONAL);
}

ServerApplication::~ServerApplication () = default;

int ServerApplication::Run () {
    
    auto service = std::make_shared<SIM::Service> (std::make_unique<SIM::COM::Factory> ());

    SystemRouter router (service);
    auto httpFactory (std::make_shared<HTTP::ClientFactory> (router));
    TCP::Server httpServer (mCommandLine.GetOption ("ip"), mCommandLine.GetOption ("port"), httpFactory);
    httpServer.Start ();

    RFC6455::Server websockServer (mCommandLine.GetOption ("ip"), mCommandLine.GetOption ("websockport"));
    websockServer.Start ();
    service->GetStream ().Pipe (&websockServer, &RFC6455::Server::BroadCast);
    
    SSE::Server sseServer (mCommandLine.GetOption ("ip"), mCommandLine.GetOption ("sseport"));
    sseServer.Start ();
    service->GetStream ().Pipe (&sseServer, &SSE::Server::BroadCast);
    
    std::string temp;
    LOGMESSAGE (OS::Log::kInfo, "Quit the app using ENTER on the command line.");
    std::getline (std::cin, temp);

    // Cannot rely on destructors to clean this up. The service is a shared_ptr
    // and will be cleaned up all the way at the end.
    if (service->IsRunning ()) service->Stop ();
    
    return 0;
}
