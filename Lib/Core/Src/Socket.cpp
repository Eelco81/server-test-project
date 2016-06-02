
#include "Log.h"
#include "Socket.h"
#include "Buffer.h"

#include <iostream>

/*
class ISocketImplementation {
    virtual bool IsConnected () const = 0;
    virtual bool IsListening () const = 0;
    virtual bool Initialize (unsigned inHandle) = 0;
    virtual bool Initialize (const std::string& inAddress, const std::string& inPort) = 0;
    virtual void Close () = 0;
    virtual bool Listen () = 0;
    virtual bool Accept (Core::Socket& outSocket) = 0;
    virtual bool Connect () = 0;
    virtual unsigned GetId () const = 0;
    virtual bool Send (const Core::Buffer& inBuffer) = 0;
    virtual bool Receive (Core::Buffer& outBuffer) = 0; 
};
*/

/* ------------------------------------- */
#if (defined _WIN32 || defined _WIN64)

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define SOCKET_HANDLE SOCKET

class Core::Socket::Implementation {

public:
    Implementation () :
        mSocketHandle (INVALID_SOCKET),
        mLatestAddrInfo (NULL),
        mIsConnected (false),
        mIsListening (false)
    {
    }
    ~Implementation () {
        Core::Log::Instance ().LogMessage (Core::Log::kTrace, std::string ("[Socket](") + std::to_string (GetId ()) + std::string (") is being destructed"));
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
        Core::Log::Instance ().LogMessage (Core::Log::kInfo, std::string ("[Socket](") + std::to_string (GetId ()) + std::string (") initialized"));
        return true;
    }

    bool Initialize (const std::string& inAddress, const std::string& inPort) {

        mIsListening = false;
        mIsConnected = false;

        struct addrinfo hints;

        ZeroMemory (&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        // Resolve the server address and port
        int res = getaddrinfo (inAddress.c_str (), inPort.c_str (), &hints, &mLatestAddrInfo);
        if (res != 0) {
            Core::Log::Instance ().LogMessage (Core::Log::kTrace, "[Socket] WinSock2::getaddrinfo failed with code " + std::to_string (WSAGetLastError ()));
            return false;
        }
        
        // Create a SOCKET for connecting to server
        mSocketHandle = socket (mLatestAddrInfo->ai_family, mLatestAddrInfo->ai_socktype, mLatestAddrInfo->ai_protocol);
        if (mSocketHandle == INVALID_SOCKET) {
            Core::Log::Instance ().LogMessage (Core::Log::kTrace, "[Socket] WinSock2::socket failed with code " + std::to_string (WSAGetLastError ()));
            return false;
        }
        
        Core::Log::Instance ().LogMessage (Core::Log::kInfo, "[Socket](" + std::to_string (GetId ()) + ") initialized at " + inAddress + ":" + inPort );
        return true;
    }

    void Close () {

        if (mIsConnected || mIsListening) {

            freeaddrinfo (mLatestAddrInfo);
            closesocket (mSocketHandle);

            Core::Log::Instance ().LogMessage (Core::Log::kInfo, std::string ("[Socket](") + std::to_string (GetId ()) + std::string (") closed"));
        }

        mIsConnected = false;
        mIsListening = false;
    }

    bool Listen () {

        if (!mIsListening) {

            int result = bind (mSocketHandle, mLatestAddrInfo->ai_addr, (int) mLatestAddrInfo->ai_addrlen);
            if (result == SOCKET_ERROR) {
                Core::Log::Instance ().LogMessage (Core::Log::kTrace, "[Socket] WinSock2::bind failed with code " + std::to_string (WSAGetLastError ()));
                return false;
            }

            result = listen (mSocketHandle, SOMAXCONN);
            if (result == SOCKET_ERROR) {
                Core::Log::Instance ().LogMessage (Core::Log::kTrace, "[Socket] WinSock2::listen failed with code " + std::to_string (WSAGetLastError ()));
                return false;
            }
        }

        mIsListening = true;
        Core::Log::Instance ().LogMessage (Core::Log::kTrace, std::string ("[Socket](") + std::to_string (GetId ()) + std::string (") listening..."));
        return true;
    }

    bool Accept (Core::Socket& outSocket) {

        if (!mIsListening) {
            return false;
        }

        SOCKET_HANDLE clientSocket = accept (mSocketHandle, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            Core::Log::Instance ().LogMessage (Core::Log::kTrace, "[Socket] WinSock2::accept failed with code " + std::to_string (WSAGetLastError ()));
            return false;
        }
        outSocket.Initialize (clientSocket);

        Core::Log::Instance ().LogMessage (Core::Log::kInfo, std::string ("[Socket](") + std::to_string (GetId ()) + std::string (") connected to socket " + std::to_string (outSocket.GetId ())));
        return true;
    }

    bool Connect () {

        if (mIsConnected) {
            return true;
        }

        // Connect to server.
        int res = connect (mSocketHandle, mLatestAddrInfo->ai_addr, (int) mLatestAddrInfo->ai_addrlen);
        if (res == SOCKET_ERROR) {
            Core::Log::Instance ().LogMessage (Core::Log::kTrace, "[Socket] WinSock2::connect failed with code " + std::to_string (WSAGetLastError ()));
            return false;
        }

        Core::Log::Instance ().LogMessage (Core::Log::kInfo, std::string ("[Socket](") + std::to_string (GetId ()) + std::string (") connected to server"));
        mIsConnected = true;
        return true;
    }

    unsigned GetId () const {
        return static_cast<unsigned> (mSocketHandle);
    }

    bool Send (const Core::Buffer& inBuffer) {
        
        if (!mIsConnected) {
            return false;
        }

        int result = send (mSocketHandle, inBuffer.GetDataPointer (), inBuffer.GetSize (), 0);
        if (result == SOCKET_ERROR) {
            Core::Log::Instance ().LogMessage (Core::Log::kTrace, "[Socket] WinSock2::send failed with code " + std::to_string (WSAGetLastError ()));
            Close ();
            return false;
        }
        Core::Log::Instance ().LogMessage (Core::Log::kTrace, std::string ("[Socket] Send ") + std::to_string (inBuffer.GetSize ()) + std::string (" bytes to client id ") + std::to_string (mSocketHandle));
        return true;
    }

    bool Receive (Core::Buffer& outBuffer) {
        
        if (!mIsConnected) {
            return false;
        }

        int result = recv (mSocketHandle, outBuffer.GetDataPointer (), outBuffer.GetMaxSize (), 0);
        if (result == 0) {
            Core::Log::Instance ().LogMessage (Core::Log::kTrace, std::string ("[Socket] WinSock2::recv received termination signal from client with id ") + std::to_string (mSocketHandle));
            Close ();
            return false;
        }
        if (result < 0) {
            Core::Log::Instance ().LogMessage (Core::Log::kTrace, "[Socket] WinSock2::recv failed with code " + std::to_string (WSAGetLastError ()));
            Close ();
            return false;
        }
        outBuffer.Resize (result);

        Core::Log::Instance ().LogMessage (Core::Log::kTrace, std::string ("[Socket] Received ") + std::to_string (result) + std::string (" bytes from client with id ") + std::to_string (mSocketHandle));
        return true;
    }

private:
    SOCKET_HANDLE mSocketHandle;
    struct addrinfo *mLatestAddrInfo;
    bool mIsConnected;
    bool mIsListening;
};

#endif

/* ------------------------------------- */

Core::Socket::Socket (const std::string& inAddress, const std::string& inPortNumber) :
    mAddress (inAddress),
    mPortNumber (inPortNumber),
    mImpl (std::make_unique <Implementation> ())
{
}

Core::Socket::~Socket () {
}

bool Core::Socket::Initialize () {
    return mImpl->Initialize (mAddress, mPortNumber);
}

bool Core::Socket::Initialize (unsigned inHandle) {
    return mImpl->Initialize (inHandle);
}

void Core::Socket::Close () {
    mImpl->Close ();
}

unsigned Core::Socket::GetId () {
    return mImpl->GetId ();
}

bool Core::Socket::Listen () {
    return mImpl->Listen ();
}

bool Core::Socket::Accept (Core::Socket& outSocket) {
    return mImpl->Accept (outSocket);
}

bool Core::Socket::Connect () {
    return mImpl->Connect ();
}

bool Core::Socket::Send (const Buffer& inBuffer) {
    return mImpl->Send (inBuffer);
}

bool Core::Socket::Receive (Buffer& outBuffer) {
    return mImpl->Receive (outBuffer);
}

bool Core::Socket::IsConnected () const {
    return mImpl->IsConnected ();
}

bool Core::Socket::IsListening () const {
    return mImpl->IsListening ();
}
