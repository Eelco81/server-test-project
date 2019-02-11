
#include "Network.h"
#include "Log.h"
#include "CommandLine.h"
#include "Application.h"

int main (int argc, char** argv) {
    
    OS::CommandLine commandLine;
    commandLine.Parse (argc, argv);

    std::string logLevel ("INFO");
    commandLine.HasOption ("loglevel", logLevel);
    
    OS::Log::Instance ().Initialize (logLevel);
    OS::Network::Initialize ();

    Application::Run (commandLine);
    
    OS::Network::Done ();
    OS::Log::Instance ().Flush ();
    
    return 0;

}
