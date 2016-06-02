
#include "Network.h"
#include "Log.h"

#include <iostream>

#if (defined _WIN32 || defined _WIN64)

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

#endif 

bool Core::Network::mInitialized (false);

void Core::Network::Initialize () {
    if (!mInitialized) {

#if (defined _WIN32 || defined _WIN64)
        WSADATA wsaData;
        int result = WSAStartup (MAKEWORD (2, 2), &wsaData);
        if (result != 0) {
            Core::Log::Instance ().LogMessage (Core::Log::kError, "[Network] WinSock2::WSAStartup failed with error: " + result);
        }
        else {
            Core::Log::Instance ().LogMessage (Core::Log::kTrace, "[Network] WinSock2::WSAStartup completed");
        }
#endif

    }
    mInitialized = true;

}

void Core::Network::Done ()  {
    if (mInitialized) {

#if (defined _WIN32 || defined _WIN64)
        WSACleanup ();
        Core::Log::Instance ().LogMessage (Core::Log::kTrace, "WinSock2::WSACleanup completed");
#endif

    }
    mInitialized = false;
}
