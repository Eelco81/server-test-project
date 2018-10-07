
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

bool OS::Network::mInitialized (false);

void OS::Network::Initialize () {
    if (!mInitialized) {

#if (defined _WIN32 || defined _WIN64)
        WSADATA wsaData;
        int result = WSAStartup (MAKEWORD (2, 2), &wsaData);
        if (result != 0) {
            LOGERROR << "[Network] WinSock2::WSAStartup failed with error: " << result;
        }
        else {
            LOGDEBUG <<"[Network] WinSock2::WSAStartup completed";
        }
#endif

    }
    mInitialized = true;

}

void OS::Network::Done ()  {
    if (mInitialized) {

#if (defined _WIN32 || defined _WIN64)
        WSACleanup ();
        LOGDEBUG << "[Network] WinSock2::WSACleanup completed";
#endif

    }
    mInitialized = false;
}
