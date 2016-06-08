
#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <string>
#include <memory>

#include "Macros.h"

namespace OS {

class Buffer;

class Socket {

    NO_COPY_CONSTRUCTORS (Socket);

public:
    Socket () = delete;
    Socket (const std::string& inAddress, const std::string& inPortNumber);
    ~Socket ();

public:
    bool Initialize ();
    bool Initialize (unsigned inHandle);

    void Close ();

    bool Listen ();
    bool Accept (Socket& outSocket);
    bool Connect ();

    bool Send (const Buffer& inBuffer);
    bool Receive (Buffer& outBuffer);

    unsigned GetId ();

    bool IsConnected () const;
    bool IsListening () const;

    inline const std::string& GetAddress () const { return mAddress; }
    inline const std::string& GetPortNumber () const { return mPortNumber; }

private:
    std::string mAddress;
    std::string mPortNumber;

    class Implementation;
    std::unique_ptr<Implementation> mImpl;
};

}

#endif