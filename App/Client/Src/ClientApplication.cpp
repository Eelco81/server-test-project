
#include <iostream>
#include <string>
#include "ClientApplication.h"

#include "Log.h"
#include "HttpRemoteClient.h"
#include "HttpRequest.h"

ClientApplication::ClientApplication (int argc, char** argv) :
    APP::Application (argc, argv, "Client") 
{
}

ClientApplication::~ClientApplication () = default;

int ClientApplication::Run () {

    std::string ip ("127.0.0.1");
    mCommandLine.HasOption ("ip", ip);

    std::string port ("1703");
    mCommandLine.HasOption ("port", port);
    
    std::string method ("GET");
    mCommandLine.HasOption ("method", method);
    
    std::string path ("/");
    mCommandLine.HasOption ("path", path);
    
    HTTP::RemoteClient client (ip, port);
    HTTP::Request request (HTTP::StringToMethod (method), path);
    
    auto response = client.Send (request);
    
    std::cout << response.GetBody () << std::endl;
    
    return 0;
}
