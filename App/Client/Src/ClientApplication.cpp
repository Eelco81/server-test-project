
#include <iostream>
#include <string>

#include "Log.h"
#include "ClientApplication.h"
#include "HttpRemoteClient.h"
#include "HttpRequest.h"

ClientApplication::ClientApplication () :
    APP::Application ("Client") 
{
    mCommandLine.AddOption ({"ip", "i"}, "127.0.0.1", OS::CommandLine::OPTIONAL);
    mCommandLine.AddOption ({"port", "p"}, "1703", OS::CommandLine::OPTIONAL);
    mCommandLine.AddOption ({"method"}, "GET", OS::CommandLine::MANDATORY);
    mCommandLine.AddOption ({"path"}, "/", OS::CommandLine::MANDATORY);
}

ClientApplication::~ClientApplication () = default;

int ClientApplication::Run () {

    HTTP::RemoteClient client (mCommandLine.GetOption ("ip"), mCommandLine.GetOption ("port"));
    HTTP::Request request (HTTP::StringToMethod (mCommandLine.GetOption ("method")), mCommandLine.GetOption ("path"));

    auto response = client.Send (request);

    std::cout << response.GetBody () << std::endl;

    return 0;
}
