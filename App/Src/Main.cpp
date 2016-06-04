
#include "Network.h"
#include "Log.h"

#include "TcpServer.h"
#include "SupportThread.h"

int main (int* argc, char** argv) {
    
    OS::Log::Instance ().Initialize (OS::Log::kTrace);
    OS::Network::Initialize ();

    APP::SupportThread supportThread;
    supportThread.Spawn ();

    TCP::Server server ("127.0.0.1", "1234");
    server.Start ();

    supportThread.Join ();
    
    OS::Network::Done ();

    return 0;
}