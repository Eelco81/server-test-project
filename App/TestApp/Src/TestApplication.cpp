
#include <iostream>
#include <string>

#include "TestApplication.h"

#include "Log.h"
#include "HttpClient.h"
#include "WebSockServer.h"
#include "SseServer.h"
#include "TcpServer.h"
#include "TestRouter.h"

TEST::Application::Application () :
    APP::Application ("test-app") 
{
    mCommandLine.AddOption ({"ip", "i"}, "127.0.0.1", OS::CommandLine::OPTIONAL);
    mCommandLine.AddOption ({"port", "p"}, "1703", OS::CommandLine::OPTIONAL);
    mCommandLine.AddOption ({"websockport", "w"}, "1704", OS::CommandLine::OPTIONAL);
    mCommandLine.AddOption ({"sseport", "w"}, "1705", OS::CommandLine::OPTIONAL);
}

TEST::Application::~Application () = default;

int TEST::Application::Run () {
    
    auto service = std::make_shared<TEST::Service> ();
    TEST::Router router (service);
  
    auto httpFactory (std::make_shared<HTTP::ClientFactory> (router));
    TCP::Server httpServer (mCommandLine.GetOption ("ip"), mCommandLine.GetOption ("port"), httpFactory);
    httpServer.Start ();
    
    RFC6455::Server websockServer (mCommandLine.GetOption ("ip"), mCommandLine.GetOption ("websockport"));
    websockServer.Start ();
    service->GetStringStream ().Pipe (&websockServer, &RFC6455::Server::BroadCast);
    service->GetBinaryStream ().Pipe (&websockServer, &RFC6455::Server::BroadCast);
    
    SSE::Server sseServer (mCommandLine.GetOption ("ip"), mCommandLine.GetOption ("sseport"));
    sseServer.Start ();
    service->GetStringStream ().Pipe (&sseServer, &SSE::Server::BroadCast);
    
    std::string temp;
    LOGMESSAGE (OS::Log::kInfo, "Quit the app using ENTER on the command line.");
    std::getline (std::cin, temp);

    return 0;
}
