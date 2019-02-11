
#include <iostream>
#include <string>
#include "Application.h"
#include "Log.h"
#include "Version.h"
#include "HttpRemoteClient.h"
#include "HttpRequest.h"
#include "SupportThread.h"

void Application::Run (const OS::CommandLine& inCommandLine) {

    APP::SupportThread supportThread;
    supportThread.Spawn ();
    
    OS::Version::SetApplicationName ("Client");

    LOGINFO << "Starting " << OS::Version::GetApplicationName () << " " << OS::Version::GetApplicationVersion ();
    
    std::string ip ("127.0.0.1");
    inCommandLine.HasOption ("ip", ip);

    std::string port ("1703");
    inCommandLine.HasOption ("port", port);
    
    std::string method ("GET");
    inCommandLine.HasOption ("method", method);
    
    std::string path ("/");
    inCommandLine.HasOption ("path", path);
    
    HTTP::RemoteClient client (ip, port);
    HTTP::Request request (HTTP::StringToMethod (method), path);
    
    auto response = client.Send (request);
    
    LOGINFO << response.GetBody();
    
}
