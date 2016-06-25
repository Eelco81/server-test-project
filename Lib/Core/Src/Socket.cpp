
#include "Log.h"
#include "Socket.h"
#include "Buffer.h"
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
    std::string ErrorMessage (const std::string& inMethod) {
        return "[Socket] CygWin::" + inMethod + " failed";
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
    std::string ErrorMessage (const std::string& inMethod) {
        return "[Socket] WinSock2::" + inMethod + " failed with code " + std::to_string (WSAGetLastError ());
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
        LOGMESSAGE (OS::Log::kTrace, std::string ("[Socket](") + std::to_string (GetId ()) + std::string (") is being destructed"));
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
            LOGMESSAGE (OS::Log::kDebug, ErrorMessage ("getaddrinfo"));
            return false;
        }
        
        // Create a SOCKET for connecting to server
        mSocketHandle = socket (mLatestAddrInfo->ai_family, mLatestAddrInfo->ai_socktype, mLatestAddrInfo->ai_protocol);
        if (mSocketHandle == INVALID_SOCKET) {
            LOGMESSAGE (OS::Log::kDebug, ErrorMessage ("socket"));
            return false;
        }
        
        int flag (1);
#if (defined __CYGWIN__ || defined __GNUC__)
        if (setsockopt (mSocketHandle, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&flag), sizeof(flag)) < 0) {
#else
        if (setsockopt (mSocketHandle, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, reinterpret_cast<char*>(&flag), sizeof(flag)) < 0) {
#endif
            LOGMESSAGE (OS::Log::kDebug, ErrorMessage ("setsocketopt"));
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
                LOGMESSAGE (OS::Log::kDebug, ErrorMessage ("bind"));
                return false;
            }

            result = listen (mSocketHandle, SOCKET_MAX_CONNECTIONS);
            if (result == SOCKET_ERROR) {
                LOGMESSAGE (OS::Log::kDebug, ErrorMessage ("listen"));
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
        if (clientSocket == INVALID_SOCKET) {
            LOGMESSAGE (OS::Log::kDebug, ErrorMessage ("accept"));
            return false;
        }
        outSocket.Initialize (clientSocket);

        LOGMESSAGE (OS::Log::kDebug, std::string ("[Socket](") + std::to_string (GetId ()) + std::string (") connected to socket " + std::to_string (outSocket.GetId ())));
        return true;
    }

    bool Connect () {

        if (mIsConnected) {
            return true;
        }

        // Connect to server.
        int res = connect (mSocketHandle, mLatestAddrInfo->ai_addr, (int) mLatestAddrInfo->ai_addrlen);
        if (res == SOCKET_ERROR) {
            LOGMESSAGE (OS::Log::kDebug, ErrorMessage ("connect")); 
            return false;
        }

        LOGMESSAGE (OS::Log::kDebug, std::string ("[Socket](") + std::to_string (GetId ()) + std::string (") connected to server"));
        mIsConnected = true;
        return true;
    }

    unsigned GetId () const {
        return static_cast<unsigned> (mSocketHandle);
    }

    bool Send (const OS::Buffer& inBuffer) {
        
        if (!mIsConnected) {
            return false;
        }

        int result = send (mSocketHandle, inBuffer.GetDataPointer (), inBuffer.GetSize (), 0);
        if (result == SOCKET_ERROR) {
            LOGMESSAGE (OS::Log::kDebug, ErrorMessage ("send"));
            Close ();
            return false;
        }
        LOGMESSAGE (OS::Log::kTrace, std::string ("[Socket] Send ") + std::to_string (inBuffer.GetSize ()) + std::string (" bytes to client id ") + std::to_string (mSocketHandle));
        return true;
    }

    bool Receive (OS::Buffer& outBuffer) {
        
        if (!mIsConnected) {
            return false;
        }

        int result = recv (mSocketHandle, outBuffer.GetDataPointer (), outBuffer.GetMaxSize (), 0);
        if (result == 0) {
            LOGMESSAGE (OS::Log::kDebug, std::string ("[Socket] Received termination signal from client with id ") + std::to_string (mSocketHandle));
            Close ();
            return false;
        }
        if (result < 0) {
            LOGMESSAGE (OS::Log::kDebug, ErrorMessage ("recv"));
            Close ();
            return false;
        }
        
        outBuffer.Resize (result);

        LOGMESSAGE (OS::Log::kTrace, std::string ("[Socket] Received ") + std::to_string (result) + std::string (" bytes from client with id ") + std::to_string (mSocketHandle));
        return true;
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

OS::Socket::~Socket () {
}

bool OS::Socket::Initialize () {
    return mImpl->Initialize (mAddress, mPortNumber);
}

bool OS::Socket::Initialize (unsigned inHandle) {
    return mImpl->Initialize (inHandle);
}

void OS::Socket::Close () {
    mImpl->Close ();
}

unsigned OS::Socket::GetId () {
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

bool OS::Socket::Send (const Buffer& inBuffer) {
    return mImpl->Send (inBuffer);
}

bool OS::Socket::Receive (Buffer& outBuffer) {
    return mImpl->Receive (outBuffer);
}

bool OS::Socket::IsConnected () const {
    return mImpl->IsConnected ();
}

bool OS::Socket::IsListening () const {
    return mImpl->IsListening ();
}
