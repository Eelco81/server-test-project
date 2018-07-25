
#include "Log.h"
#include "Socket.h"
#include <string.h>

#include <atomic>

#define SOCKET_MAX_CONNECTIONS 5

/* ------------------------------------- */
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
        return std::string ("[Socket](") + std::to_string (inId) + std::string (") CygWin::" + inMethod + " failed");
    }
}

/* ------------------------------------- */
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

/* ------------------------------------- */
#else 
static_assert (true, "Incompatible compiler");
#endif
/* ------------------------------------- */

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
        //LOGMESSAGE (OS::Log::kTrace, std::string ("[Socket](") + std::to_string (GetId ()) + std::string (") is being destructed"));
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
        LOGMESSAGE (OS::Log::kDebug, std::string ("[Socket](") + std::to_string (GetId ()) + std::string (") initialized"));
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
            LOGMESSAGE (OS::Log::kDebug, ErrorMessage ("getaddrinfo", GetId()));
            return false;
        }
        
        // Create a SOCKET for connecting to server
        mSocketHandle = socket (mLatestAddrInfo->ai_family, mLatestAddrInfo->ai_socktype, mLatestAddrInfo->ai_protocol);
        if (mSocketHandle == INVALID_SOCKET) {
            LOGMESSAGE (OS::Log::kDebug, ErrorMessage ("socket", GetId()));
            return false;
        }
        
        int flag (1);
#if (defined __CYGWIN__ || defined __GNUC__)
        if (setsockopt (mSocketHandle, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&flag), sizeof(flag)) < 0) {
#else
        if (setsockopt (mSocketHandle, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, reinterpret_cast<char*>(&flag), sizeof(flag)) < 0) {
#endif
            LOGMESSAGE (OS::Log::kDebug, ErrorMessage ("setsocketopt", GetId()));
            return false;
        }

        LOGMESSAGE (OS::Log::kDebug, "[Socket](" + std::to_string (GetId ()) + ") initialized at " + inAddress + ":" + inPort);
        return true;
    }

    void Close () {

        if (mIsConnected || mIsListening) {

            freeaddrinfo (mLatestAddrInfo);

            shutdown (mSocketHandle, 2);

#if (defined __CYGWIN__ || defined __GNUC__)
            close (mSocketHandle);
#else 
            closesocket (mSocketHandle);
#endif
            LOGMESSAGE (OS::Log::kDebug, std::string ("[Socket](") + std::to_string (GetId ()) + std::string (") closed"));
        }

        mIsConnected = false;
        mIsListening = false;
    }

    bool Listen () {

        if (!mIsListening) {

            int result = bind (mSocketHandle, mLatestAddrInfo->ai_addr, (int) mLatestAddrInfo->ai_addrlen);
            if (result == SOCKET_ERROR) {
                LOGMESSAGE (OS::Log::kDebug, ErrorMessage ("bind", GetId()));
                return false;
            }

            result = listen (mSocketHandle, SOCKET_MAX_CONNECTIONS);
            if (result == SOCKET_ERROR) {
                LOGMESSAGE (OS::Log::kDebug, ErrorMessage ("listen", GetId()));
                return false;
            }
        }

        mIsListening = true;
        LOGMESSAGE (OS::Log::kDebug, std::string ("[Socket](") + std::to_string (GetId ()) + std::string (") listening..."));
        return true;
    }

    bool Accept (OS::Socket& outSocket) {

        if (!mIsListening) {
            return false;
        }

        SOCKET_HANDLE clientSocket = accept (mSocketHandle, NULL, NULL);
        if (clientSocket <= 0) {
            LOGMESSAGE (OS::Log::kDebug, ErrorMessage ("accept", GetId()));
            return false;
        }
        outSocket.Initialize (clientSocket);

        LOGMESSAGE (OS::Log::kDebug, std::string ("[Socket](") + std::to_string (GetId ()) + std::string (") accepted socket " + std::to_string (outSocket.GetId ())));
        return true;
    }

    bool Connect () {

        if (mIsConnected) {
            return true;
        }

        // Connect to server.
        int res = connect (mSocketHandle, mLatestAddrInfo->ai_addr, (int) mLatestAddrInfo->ai_addrlen);
        if (res == SOCKET_ERROR) {
            LOGMESSAGE (OS::Log::kDebug, ErrorMessage ("connect", GetId())); 
            return false;
        }

        LOGMESSAGE (OS::Log::kDebug, std::string ("[Socket](") + std::to_string (GetId ()) + std::string (") connected to server"));
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

        int result = send (mSocketHandle, inBuffer, inBufferSize, 0);
        if (result == SOCKET_ERROR) {
            LOGMESSAGE (OS::Log::kDebug, ErrorMessage ("send", GetId()));
            Close ();
            return -1;
        }
        if (result == 0) {
            LOGMESSAGE (OS::Log::kDebug, std::string ("[Socket](") + std::to_string (GetId ()) + std::string (") Failed to send any bytes"));
            Close ();
            return -1;
        }
 
        LOGMESSAGE (OS::Log::kTrace, std::string ("[Socket](") + std::to_string (GetId ()) + std::string (") Send ") + std::to_string (result) + std::string (" bytes"));
        return result;
    }

    int Receive (uint8_t* ioBuffer, std::size_t inBufferSize) {
        
        if (!mIsConnected) {
            return false;
        }

        int result = recv (mSocketHandle, ioBuffer, inBufferSize, 0);
        if (result == 0) {
            LOGMESSAGE (OS::Log::kDebug, std::string ("[Socket](") + std::to_string (GetId ()) + std::string (") Received termination signal"));
            Close ();
            return -1;
        }
        if (result < 0) {
            LOGMESSAGE (OS::Log::kDebug, ErrorMessage ("recv", GetId()));
            Close ();
            return -1;
        }
        
        LOGMESSAGE (OS::Log::kTrace, std::string ("[Socket](") + std::to_string (GetId ()) + std::string (") Received ") + std::to_string (result) + std::string (" bytes"));
        return result;
    }

private:
    SOCKET_HANDLE mSocketHandle;
    struct addrinfo *mLatestAddrInfo;
    std::atomic<bool> mIsConnected;
    std::atomic<bool> mIsListening;
};

/* ------------------------------------- */

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