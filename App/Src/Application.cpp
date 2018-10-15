
#include <iostream>
#include <string>
#include "Application.h"
#include "Log.h"
#include "Version.h"
#include "Socket.h"
#include "HttpClient.h"
#include "WebSockClient.h"
#include "TcpServer.h"
#include "SupportThread.h"
#include "SimService.h"
#include "SimFactory.h"
#include "SimBlock.h"

#include "Router.h"

namespace {

class DummyBlock : public SIM::Block {
public:
    DummyBlock (const std::string& inName) : 
        SIM::Block (inName)
    {
        AddOutPort<double> (&mOutput, "value");
    }
    ~DummyBlock () = default;
    void Step (double mTime) {
        mOutput = std::sin (mTime);
        LOGTRACE << "TICK " << mTime << " " << mOutput;
    }
private:
    double mOutput;
};

class DummyFactory : public SIM::Factory {
    std::unique_ptr<SIM::Block> CreateBlock (const std::string& inName, const std::string& inType) {
        return std::make_unique <DummyBlock> (inName);
    };
};

} // end anonymous namespace

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

    auto service = std::make_shared<SIM::Service> (std::make_unique<DummyFactory> ());

    SystemRouter router (service);
    auto httpFactory (std::make_shared<HTTP::ClientFactory> (router));
    TCP::Server httpServer (ip, port, httpFactory);
    httpServer.Start ();

    auto websockFactory (std::make_shared<RFC6455::ClientFactory> ());
    TCP::Server websockServer (ip, websockport, websockFactory);
    websockServer.Start ();

    std::string name;
    LOGMESSAGE (OS::Log::kInfo, "Quit the app using ENTER on the command line.");
    std::getline (std::cin, name);

}