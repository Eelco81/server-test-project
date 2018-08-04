
#include <iostream>

#include "Network.h"
#include "Log.h"
#include "CommandLine.h"
#include "Socket.h"
#include "TcpClient.h"
#include "TcpServer.h"
#include "TcpClientFactory.h"
#include "SupportThread.h"

namespace {

class EchoClient : public TCP::Client {
public:
    EchoClient (std::unique_ptr<OS::Socket> inSocket) :
       TCP::Client (std::move (inSocket)) 
    {
    }
    void OnReceived (const std::vector<uint8_t>& inBuffer) override {
        Send (inBuffer);
    }
};

class EchoClientFactory : public TCP::ClientFactory {
public:
    std::unique_ptr<TCP::Client> Create (std::unique_ptr<OS::Socket> inSocket) const override {
        return std::make_unique<EchoClient> (std::move (inSocket));
    }
};

} // end anonymous namespace

int main (int argc, char** argv) {
    
    OS::CommandLine commandLine;
    commandLine.Parse (argc, argv);

    std::string ip ("127.0.0.1");
    commandLine.HasOption ("ip", ip);

    std::string port ("1234");
    commandLine.HasOption ("port", port);

    std::string logLevel ("INFO");
    commandLine.HasOption ("loglevel", logLevel);

    OS::Log::Instance ().Initialize (logLevel);
    OS::Network::Initialize ();

    APP::SupportThread supportThread;
    supportThread.Spawn ();

    auto factory (std::make_unique<EchoClientFactory> ());
    TCP::Server server (ip, port, std::move (factory));
    server.Start ();

    std::string name;
    LOGMESSAGE (OS::Log::kInfo, "Quit the app using ENTER on the command line.");
    std::getline (std::cin, name);
    
    server.Stop();
    supportThread.Kill ();
    supportThread.Join ();
    
    OS::Network::Done ();

    return 0;
}