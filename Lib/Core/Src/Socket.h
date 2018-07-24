
#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include "Macros.h"

namespace OS {

class Socket {

    NO_COPY_CONSTRUCTORS (Socket);

public:
    Socket () = delete;
    Socket (const std::string& inAddress, const std::string& inPortNumber);
    ~Socket ();

public:
    bool Initialize ();
    bool Initialize (int inHandle);

    void Close ();

    bool Listen ();
    bool Accept (Socket& outSocket);
    bool Connect ();

    int Send (const std::vector<uint8_t>& inBuffer);
    int Send (const uint8_t* inBuffer, std::size_t inBufferSize);

    int Receive (std::vector<uint8_t>& ioBuffer);
    int Receive (uint8_t* ioBuffer, std::size_t inBufferSize);

    int GetId ();

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