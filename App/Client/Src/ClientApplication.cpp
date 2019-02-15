
#include <iostream>
#include <string>
#include "ClientApplication.h"

#include "Log.h"
#include "HttpRemoteClient.h"
#include "HttpRequest.h"

ClientApplication::ClientApplication () :
    APP::Application ("Client") 
{
    mCommandLine.AddOption ("ip", "127.0.0.1", OS::CommandLine::OPTIONAL);
    mCommandLine.AddOption ("port", "1703", OS::CommandLine::OPTIONAL);
    mCommandLine.AddOption ("method", "GET", OS::CommandLine::OPTIONAL);
    mCommandLine.AddOption ("path", "/", OS::CommandLine::OPTIONAL);
}

ClientApplication::~ClientApplication () = default;

int ClientApplication::Run () {

    HTTP::RemoteClient client (mCommandLine.GetOption ("ip"), mCommandLine.GetOption ("port"));
    HTTP::Request request (HTTP::StringToMethod (mCommandLine.GetOption ("method")), mCommandLine.GetOption ("path"));
    
    auto response = client.Send (request);
    
    std::cout << response.GetBody () << std::endl;
    
    return 0;
}
