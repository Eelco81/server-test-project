
#include "Log.h"
#include "Socket.h"
#include <string.h>

#include <iostream>
#include <atomic>

#define SOCKET_MAX_CONNECTIONS 20

#if (defined __CYGWIN__ || defined __GNUC__)

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define SOCKET_HANDLE int
#define INVALID_SOCKET 0
#define SOCKET_ERROR -1

namespace {
    std::string ErrorMessage (const std::string& inMethod, int inId) {
        return std::string ("[Socket](") + std::to_string (inId) + std::string (") Gnu::" + inMethod + " failed");
    }
}

#elif _MSC_VER >= 1800

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define SOCKET_HANDLE SOCKET

namespace {
    std::string ErrorMessage (const std::string& inMethod, int inId) {
        return std::string ("[Socket](") + std::to_string (inId) + std::string (") WinSock2::") + inMethod + std::string(" failed with code ") + std::to_string (WSAGetLastError ());
    }
}

#else 
static_assert (true, "Incompatible compiler");
#endif

class OS::Socket::Implementation {

public:
    Implementation () :
        mSocketHandle (INVALID_SOCKET),
        mLatestAddrInfo (NULL),
        mIsConnected (false),
        mIsListening (false)
    {
    }
    ~Implementation () {
        Close ();
    }

public:

    bool IsConnected () const {
        return mIsConnected;
    }

    bool IsListening () const {
        return mIsListening;
    }

    bool Initialize (SOCKET_HANDLE inSocketHandle) {
        mIsListening = false;
        mIsConnected = true;
        mSocketHandle = inSocketHandle;
        LOGDEBUG << "[Socket](" << GetId () << ") initialized";
        return true;
    }

    bool Initialize (const std::string& inAddress, const std::string& inPort) {

        mIsListening = false;
        mIsConnected = false;

        struct addrinfo hints;
        
#if (defined __CYGWIN__ || defined __GNUC__)
        ::memset (&hints, 0, sizeof (hints));
#else
        ZeroMemory (&hints, sizeof (hints));
#endif
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        // Resolve the server address and port
        int res = getaddrinfo (inAddress.c_str (), inPort.c_str (), &hints, &mLatestAddrInfo);
        if (res != 0) {
            LOGDEBUG << ErrorMessage ("getaddrinfo", GetId());
            return false;
        }
        
        // Create a SOCKET for connecting to server
        mSocketHandle = socket (mLatestAddrInfo->ai_family, mLatestAddrInfo->ai_socktype, mLatestAddrInfo->ai_protocol);
        if (mSocketHandle == INVALID_SOCKET) {
            LOGDEBUG << ErrorMessage ("socket", GetId());
            return false;
        }
        
        int flag (1);
#if (defined __CYGWIN__ || defined __GNUC__)
        if (setsockopt (mSocketHandle, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&flag), sizeof(flag)) < 0) {
#else
        if (setsockopt (mSocketHandle, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, reinterpret_cast<char*>(&flag), sizeof(flag)) < 0) {
#endif
            LOGDEBUG << ErrorMessage ("setsocketopt", GetId());
            return false;
        }

        LOGDEBUG << "[Socket](" << GetId () << ") initialized at " << inAddress << ":" << inPort;
        return true;
    }

    void Close () {

        if (mIsConnected || mIsListening) {

            if (mLatestAddrInfo != NULL) {
                freeaddrinfo (mLatestAddrInfo);
                mLatestAddrInfo = NULL;
            }
            
            shutdown (mSocketHandle, 2);

#if (defined __CYGWIN__ || defined __GNUC__)
            close (mSocketHandle);
#else 
            closesocket (mSocketHandle);
#endif
            LOGDEBUG << "[Socket](" << GetId () << ") closed";
        }

        mIsConnected = false;
        mIsListening = false;
    }

    bool Listen () {

        if (!mIsListening) {

            int result = bind (mSocketHandle, mLatestAddrInfo->ai_addr, (int) mLatestAddrInfo->ai_addrlen);
            if (result == SOCKET_ERROR) {
                LOGDEBUG << ErrorMessage ("bind", GetId());
                return false;
            }

            result = listen (mSocketHandle, SOCKET_MAX_CONNECTIONS);
            if (result == SOCKET_ERROR) {
                LOGDEBUG << ErrorMessage ("listen", GetId());
                return false;
            }
        }

        mIsListening = true;
        LOGDEBUG << "[Socket](" << GetId () << ") listening...";
        return true;
    }

    bool Accept (OS::Socket& outSocket) {

        if (!mIsListening) {
            return false;
        }

        SOCKET_HANDLE clientSocket = accept (mSocketHandle, NULL, NULL);
        if (clientSocket <= 0) {
            LOGDEBUG << ErrorMessage ("accept", GetId ());
            return false;
        }
        outSocket.Initialize (clientSocket);

        LOGDEBUG << "[Socket](" << GetId () << ") accepted socket " << outSocket.GetId ();
        return true;
    }

    bool Connect () {

        if (mIsConnected) {
            return true;
        }

        // Connect to server.
        int res = connect (mSocketHandle, mLatestAddrInfo->ai_addr, (int) mLatestAddrInfo->ai_addrlen);
        if (res == SOCKET_ERROR) {
            LOGDEBUG << ErrorMessage ("connect", GetId ()); 
            return false;
        }

        LOGDEBUG << "[Socket](" << GetId () << ") connected to server";
        mIsConnected = true;
        return true;
    }

    int GetId () const {
        return mSocketHandle;
    }

    int Send (const uint8_t* inBuffer, std::size_t inBufferSize)  {
        
        if (!mIsConnected) {
            return false;
        }

        int result = send (mSocketHandle, reinterpret_cast<const char*>(inBuffer), inBufferSize, 0);
        if (result == SOCKET_ERROR) {
            LOGDEBUG << ErrorMessage ("send", GetId ());
            Close ();
            return -1;
        }
        if (result == 0) {
            LOGDEBUG << "[Socket](" << GetId () << ") Failed to send any bytes";
            Close ();
            return -1;
        }
 
        LOGTRACE << "[Socket](" << GetId () << ") Send " << result << " bytes";
        return result;
    }

    int Receive (uint8_t* ioBuffer, std::size_t inBufferSize) {
        
        if (!mIsConnected) {
            return false;
        }

        int result = recv (mSocketHandle, reinterpret_cast<char*>(ioBuffer), inBufferSize, 0);
        if (result == 0) {
            LOGDEBUG << "[Socket](" << GetId () << ") Received termination signal";
            Close ();
            return -1;
        }
        if (result < 0) {
            LOGDEBUG << ErrorMessage ("recv", GetId());
            Close ();
            return -1;
        }
        
        LOGTRACE << "[Socket](" << GetId () << ") Received " << result << " bytes";
        return result;
    }

private:
    SOCKET_HANDLE mSocketHandle;
    struct addrinfo *mLatestAddrInfo;
    std::atomic<bool> mIsConnected;
    std::atomic<bool> mIsListening;
};

OS::Socket::Socket (const std::string& inAddress, const std::string& inPortNumber) :
    mAddress (inAddress),
    mPortNumber (inPortNumber),
    mImpl (std::make_unique <Implementation> ())
{
}

OS::Socket::~Socket () = default;

bool OS::Socket::Initialize () {
    return mImpl->Initialize (mAddress, mPortNumber);
}

bool OS::Socket::Initialize (int inHandle) {
    return mImpl->Initialize (inHandle);
}

void OS::Socket::Close () {
    mImpl->Close ();
}

int OS::Socket::GetId () {
    return mImpl->GetId ();
}

bool OS::Socket::Listen () {
    return mImpl->Listen ();
}

bool OS::Socket::Accept (OS::Socket& outSocket) {
    return mImpl->Accept (outSocket);
}

bool OS::Socket::Connect () {
    return mImpl->Connect ();
}

int OS::Socket::Send (const std::vector<uint8_t>& inBuffer) {
    return mImpl->Send (inBuffer.data(), inBuffer.size());
}

int OS::Socket::Send (const uint8_t* inBuffer, std::size_t inBufferSize) {
    return mImpl->Send (inBuffer, inBufferSize);
}

int OS::Socket::Receive (std::vector<uint8_t>& ioBuffer) {
    return mImpl->Receive (ioBuffer.data(), ioBuffer.size());
}

int OS::Socket::Receive (uint8_t* ioBuffer, std::size_t inBufferSize) {
    return mImpl->Receive (ioBuffer, inBufferSize);
}

bool OS::Socket::IsConnected () const {
    return mImpl->IsConnected ();
}

bool OS::Socket::IsListening () const {
    return mImpl->IsListening ();
}