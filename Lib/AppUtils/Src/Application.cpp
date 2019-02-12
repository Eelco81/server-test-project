
#include "Application.h"
#include "Network.h"
#include "Log.h"
#include "Version.h"

APP::Application::Application (int argc, char** argv, const std::string& inAppName) 
{
    mSupportThread.Spawn ();
    mCommandLine.Parse (argc, argv);
    
    std::string logLevel ("INFO");
    mCommandLine.HasOption ("loglevel", logLevel);
    
    OS::Log::Instance ().Initialize (logLevel);
    OS::Network::Initialize ();
    
    OS::Version::SetApplicationName (inAppName);
    LOGINFO << "Starting " << OS::Version::GetApplicationName () << " " << OS::Version::GetApplicationVersion ();
}

APP::Application::~Application () {

    LOGINFO << "Stopped " << OS::Version::GetApplicationName () << " " << OS::Version::GetApplicationVersion ();
    
    OS::Network::Done ();
    OS::Log::Instance ().Flush ();
}

