

#ifndef _SSE_REMOTECLIENT_H_
#define _SSE_REMOTECLIENT_H_

#include <string>
#include <memory>
#include <mutex>
#include <condition_variable>

#include "TcpClient.h"
#include "TcpPacketEncoders.h"
#include "HttpRequestEncoder.h"
#include "HttpResponseDecoder.h"
#include "SseEventDecoder.h"

namespace HTTP {
    struct Request;
    struct Response;
}

namespace SSE {

/**
 * The SSE::RemoteClient class forwards establishes an Server-Sent-Events
 * connection to a remote server.
 */
class RemoteClient : public TCP::Client {

public:
    /**
     * Create a SSE remote client, which connects to inAddress:inPort
     */
    RemoteClient (std::string inAddress, std::string inPort);
    
    /**
     * Virtual destructor
     */
    virtual ~RemoteClient ();
    
    /**
     * This signal is emitted when a new event is available
     */
    OS::Signal<const std::string&> sEventAvailable;

private:
    /**
     * Process an incoming handshake HTTP Response (from HTTP::ResponseDecoder)
     */
    void HandleHandshake (const HTTP::Response& inResponse);
    
private:
    /**
     * MessageStreams which link ReadStream to the WriteStream
     */
    TCP::Packet2StringEncoder mPacket2String;
    TCP::String2PacketEncoder mString2Packet;
    HTTP::RequestEncoder mRequestEncoder;
    HTTP::ResponseDecoder mResponseDecoder;
    EventDecoder mEventDecoder;
    
};

} // end namespace SSE

#endif // _SSE_REMOTECLIENT_H_
