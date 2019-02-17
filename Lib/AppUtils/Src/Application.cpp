
#include "Application.h"
#include "Network.h"
#include "Log.h"
#include "Version.h"
#include <iostream>

APP::Application::Application (const std::string& inAppName) 
{
    mSupportThread.Spawn ();
    
    mCommandLine.AddOption ({"loglevel", "l"}, "NONE", OS::CommandLine::OPTIONAL);
    mCommandLine.AddOption ({"help", "h"}, "", OS::CommandLine::OPTIONAL);
    mCommandLine.AddOption ({"version", "v"}, "", OS::CommandLine::OPTIONAL);
    
    OS::Network::Initialize ();
    OS::Version::SetApplicationName (inAppName);
}

APP::Application::~Application () {

    LOGINFO << "Stopped " << OS::Version::GetApplicationName () << " " << OS::Version::GetApplicationVersion ();
    
    OS::Network::Done ();
    OS::Log::Instance ().Flush ();
}

APP::Application::Execute (int argc, char** argv) {
    
    const auto correctLine (mCommandLine.Parse (argc, argv));
    
    if (mCommandLine.ContainsOption ("version")) {
        OS::Log::Instance ().Initialize ("NONE");
        std::cout << mCommandLine.GetVersionText () << std::endl;
        return 1;
    }
    
    if (mCommandLine.ContainsOption ("help")) {
        OS::Log::Instance ().Initialize ("NONE");
        std::cout << mCommandLine.GetHelpText () << std::endl;
        return 1;
    }
    
    if (!correctLine) {
        OS::Log::Instance ().Initialize ("NONE");
        std::cout << "Incorrect command line, please call \"-help\"." << std::endl;
        return 1;
    }
    
    OS::Log::Instance ().Initialize (mCommandLine.GetOption ("loglevel"));
    
    LOGINFO << "Starting " << OS::Version::GetApplicationName () << " " << OS::Version::GetApplicationVersion ();
    
    return Run ();
}