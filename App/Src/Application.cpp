
#include <iostream>
#include <string>
#include "Application.h"
#include "Log.h"
#include "Version.h"
#include "Socket.h"
#include "HttpClient.h"
#include "WebSockServer.h"
#include "TcpServer.h"
#include "SupportThread.h"
#include "SimService.h"
#include "SimComFactory.h"
#include "Router.h"

void Application::Run (const OS::CommandLine& inCommandLine) {

    LOGINFO << "Starting " << OS::Version::GetApplicationName () << " " << OS::Version::GetApplicationVersion ();

    std::string ip ("127.0.0.1");
    inCommandLine.HasOption ("ip", ip);

    std::string port ("1703");
    inCommandLine.HasOption ("port", port);

    std::string websockport ("1704");
    inCommandLine.HasOption ("websockport", websockport);

    APP::SupportThread supportThread;
    supportThread.Spawn ();

    auto service = std::make_shared<SIM::Service> (std::make_unique<SIM::COM::Factory> ());

    SystemRouter router (service);
    auto httpFactory (std::make_shared<HTTP::ClientFactory> (router));
    TCP::Server httpServer (ip, port, httpFactory);
    httpServer.Start ();

    RFC6455::Server websockServer (ip, websockport);
    websockServer.Start ();

    service->GetSampleStream ().Pipe (&websockServer, &RFC6455::Server::BroadCast);
    
    std::string name;
    LOGMESSAGE (OS::Log::kInfo, "Quit the app using ENTER on the command line.");
    std::getline (std::cin, name);

    service->GetSampleStream ().Clear ();
}