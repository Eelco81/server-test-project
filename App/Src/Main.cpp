
#include "Network.h"
#include "Log.h"

#include "TcpServer.h"
#include "SupportThread.h"

int main (int* argc, char** argv) {
    
    Core::Log::Instance ().Initialize (Core::Log::kTrace);
    Core::Network::Initialize ();

    APP::SupportThread supportThread;
    supportThread.Spawn ();

    TCP::Server server ("127.0.0.1", "1234");
    server.Start ();

    supportThread.Join ();
    
    Core::Network::Done ();

    return 0;
}